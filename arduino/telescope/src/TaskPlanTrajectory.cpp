#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"

// File local variables
uint8_t lastSeqNumReceived{0};
TrajectoryPart_t trajectoryPart{0, 0, 0, {{0.0, 0.0, 0.0}}};

bool trajRunning{false};
unsigned long timeToNextEntry{0};
uint8_t currEntry{0};
uint8_t numEntries{0};
uint8_t numParts{0};
TrajectoryEntry_t trajectory[MAX_ENTRIES_PER_TRAJECTORY_PART * MAX_TRAJECTORY_PARTS]{0};
unsigned long startTime{0};

/// Process the trajectory by iterating through each entry and sending commands
/// to the servos at the specified time
///
/// @param[in] trajectory The trajectory to process (array of entries)
/// @param[in] numEntries The number of entries in the trajectory
/// @param[in] startTime The start time of the trajectory in milliseconds
///                      Used to calculate relative time for each entry
/// @param[in] moveCmdBufferHandle The message buffer handle to send move commands
void processTrajectory(const TrajectoryEntry_t* trajectory, const uint8_t numEntries, 
                       const unsigned long& startTime, MessageBufferHandle_t moveCmdBufferHandle)
{
  trajRunning = true;

  // Process the trajectory entries
  for (currEntry = 0; currEntry < numEntries; ++currEntry)
  {
    unsigned long nowRel = millis() - startTime;
    if (nowRel < trajectory[currEntry].t * 1000)
    {
      // Wait until the specified time for this entry
      timeToNextEntry = trajectory[currEntry].t * 1000 - nowRel;
      vTaskDelay(pdMS_TO_TICKS(timeToNextEntry));
    }

    DEBUG_TRAJECTORY("Processing trajectory entry: " + 
                      String(trajectory[currEntry].t) + ", " + 
                      String(trajectory[currEntry].az) + ", " + 
                      String(trajectory[currEntry].el));
    
    MoveServoCmd_t moveCmd{trajectory[currEntry].az, trajectory[currEntry].el};
    xMessageBufferSend(moveCmdBufferHandle, 
                       reinterpret_cast<const uint8_t*>(&moveCmd), 
                       sizeof(MoveServoCmd_t), 0);
  }
}

/// Reset the trajectory variables in the task
void resetTrajectory()
{
  lastSeqNumReceived = 0;
  numEntries = 0;
  currEntry = 0;
  numParts = 0;
  timeToNextEntry = 0;
  trajRunning = false;
}

void taskPlanTrajectory(void* params)
{
  DEBUG_ENTER("taskPlanTrajectory()");

  PlanTrajectoryParams* planParams = static_cast<PlanTrajectoryParams*>(params);
  MessageBufferHandle_t msgBufferHandle = planParams->msgBufferHandle;
  MessageBufferHandle_t moveCmdBufferHandle = planParams->moveCmdBufferHandle;
  Telemetry* telemetry = planParams->telemetry;

  // Register task telemetry
  telemetry->registerTelemFieldTrajRunning(&trajRunning);
  telemetry->registerTelemFieldTrajNumEntries(&numEntries);
  telemetry->registerTelemFieldTrajCurrEntry(&currEntry);
  telemetry->registerTelemFieldTimeToNextEntry(&timeToNextEntry);

  FOREVER
  {
    DEBUG_HEARTBEAT("PlanTrajectory");

    // Wait for a new trajectory command
    size_t bytesRead = xMessageBufferReceive(msgBufferHandle, reinterpret_cast<uint8_t*>(&trajectoryPart), 
                                             sizeof(TrajectoryPart_t), portMAX_DELAY);
    if (bytesRead >= sizeof(TrajectoryHeader_t))
    {
      DEBUG_TRAJECTORY("Received trajectory part: " + 
                        String(trajectoryPart.header.seqNum) + ", " + 
                        String(trajectoryPart.header.totalParts) + ", " + 
                        String(trajectoryPart.header.numEntries));

      // Verify expected length was received
      const uint16_t expectedSize = sizeof(TrajectoryHeader_t) +
                                    sizeof(TrajectoryEntry_t) * trajectoryPart.header.numEntries;
      if (bytesRead != expectedSize)
      {
        DEBUG_PRINTLN("ERROR - Only received " + String(bytesRead) + " for trajectory part."
                      " Expected " + String(expectedSize));
        resetTrajectory();
        continue;
      }

      // Check if sequence number is next expected
      if (trajectoryPart.header.seqNum != lastSeqNumReceived + 1)
      {
        DEBUG_PRINTLN("ERROR - Received trajectory part out of order! Expected " + 
                      String(lastSeqNumReceived + 1) + ", got " + String(trajectoryPart.header.seqNum));
        resetTrajectory();
        continue;
      }
      lastSeqNumReceived = trajectoryPart.header.seqNum;

      if (trajectoryPart.header.seqNum == 1)
      {
        if (trajectoryPart.header.totalParts > MAX_TRAJECTORY_PARTS)
        {
          DEBUG_PRINTLN("ERROR - Trajectory has too many parts! " + 
                        String(trajectoryPart.header.totalParts) + " > " + 
                        String(MAX_TRAJECTORY_PARTS));
          resetTrajectory();
          continue;
        }

        // First part of the trajectory received. Mark start time for relative time.
        // Also record the number of parts in the trajectory
        startTime = millis();
        numParts = trajectoryPart.header.totalParts;
      }

      if (trajectoryPart.header.seqNum > numParts)
      {
        DEBUG_PRINTLN("ERROR - Received trajectory part with sequence number " + 
                      String(trajectoryPart.header.seqNum) + 
                      " greater than total parts " + String(numParts));
        resetTrajectory();
        continue;
      }

      if (trajectoryPart.header.totalParts != numParts)
      {
        DEBUG_PRINTLN("ERROR - Received trajectory part with total parts " + 
                      String(trajectoryPart.header.totalParts) + 
                      " does not match expected " + String(numParts));
        resetTrajectory();
        continue;
      }

      if (trajectoryPart.header.numEntries > MAX_ENTRIES_PER_TRAJECTORY_PART)
      {
        DEBUG_PRINTLN("ERROR - Received trajectory part with too many entries! " + 
                      String(trajectoryPart.header.numEntries) + " > " + 
                      String(MAX_ENTRIES_PER_TRAJECTORY_PART));
        resetTrajectory();
        continue;
      }

      // Insert the entries into the trajectory
      for (uint8_t i = 0; i < trajectoryPart.header.numEntries; ++i)
      {
        trajectory[numEntries++] = trajectoryPart.entries[i];
      }

      if (trajectoryPart.header.seqNum == numParts)
      {
        // Last part of the trajectory received. Process the trajectory
        DEBUG_TRAJECTORY("Received complete trajectory with " + String(numEntries) + " entries.");
        processTrajectory(trajectory, numEntries, startTime, moveCmdBufferHandle);
        resetTrajectory(); // We are done with this trajectory
      }
    }
    else
    {
      DEBUG_PRINTLN("ERROR - Received trajectory part sized " + String(bytesRead) + 
                    " bytes, expected " + String(sizeof(TrajectoryPart_t)) + " bytes!");
    }
  }

  DEBUG_EXIT("taskPlanTrajectory()");
}
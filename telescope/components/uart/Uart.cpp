#include <esp_log.h>
#include <driver/gpio.h>

#include "Uart.h"

UART::UART(const uart_port_t& uartPort, 
           const int& txPin, const int& rxPin, 
           const int& rtsPin, const int& ctsPin,
           const int& baudRate, 
           const uart_word_length_t& dataBits,
           const uart_parity_t& parity, 
           const uart_stop_bits_t& stopBits,
           const uart_hw_flowcontrol_t& flowControl,
           const uint8_t rxFlowCtrlThresh,
           const uart_sclk_t& sourceClk,
           const bool allowPd) : 
            uartPort(uartPort), txPin(txPin), rxPin(rxPin),
            rtsPin(rtsPin), ctsPin(ctsPin),
            uartConfig({baudRate, dataBits, parity, stopBits, 
                        flowControl, rxFlowCtrlThresh, sourceClk, {allowPd}})
{
  // Initialize the UART
  init();

  // Start the UART communication loop
  start();
}

UART::UART(const uart_port_t& uartPort, 
           const int& txPin, const int& rxPin, 
           const int& rtsPin, const int& ctsPin, 
           const uart_config_t& config) : 
            uartPort(uartPort), txPin(txPin), rxPin(rxPin),
            rtsPin(rtsPin), ctsPin(ctsPin), uartConfig(config)
{
  // Initialize the UART
  ESP_ERROR_CHECK(init());

  // Start the UART communication loop
  start();
}

UART::~UART()
{
  // Stop the UART communication
  stop();

  // Deinitialize the UART port
  uart_driver_delete(uartPort);
}

esp_err_t UART::init()
{
  // Configure the UART parameters
  esp_err_t err = uart_param_config(uartPort, &uartConfig);
  if (err != ESP_OK)
  {
    ESP_LOGE("UART", "Failed to configure UART parameters: %s", esp_err_to_name(err));
    return err;
  }

  // Verify that selected pins are valid
  if (!GPIO_IS_VALID_GPIO(txPin) || !GPIO_IS_VALID_GPIO(rxPin) ||
      (rtsPin != UART_PIN_NO_CHANGE && !GPIO_IS_VALID_GPIO(rtsPin)) ||
      (ctsPin != UART_PIN_NO_CHANGE && !GPIO_IS_VALID_GPIO(ctsPin)))
  {
    ESP_LOGE("UART", "Invalid GPIO pins specified for UART");
    return ESP_ERR_INVALID_ARG;
  }

  // Set the UART pins
  err = uart_set_pin(uartPort, txPin, rxPin, rtsPin, ctsPin);
  if (err != ESP_OK)
  {
    ESP_LOGE("UART", "Failed to set UART pins: %s", esp_err_to_name(err));
    return err;
  }

  // Install the UART driver
  err = uart_driver_install(uartPort, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 
                            UART_EVENT_QUEUE_SIZE, &uartQueue, 0);
  if (err != ESP_OK)
  {
    ESP_LOGE("UART", "Failed to install UART driver: %s", esp_err_to_name(err));
    return err;
  }

  return ESP_OK;
}

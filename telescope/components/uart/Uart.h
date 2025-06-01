#ifndef __UART_H__
#define __UART_H__

#include <array>
#include <driver/uart.h>
#include <esp_err.h>
#include <thread>

class UART
{
public:
  static constexpr uart_config_t DEFAULT_UART_CONFIG{115200,
                                                     UART_DATA_8_BITS,
                                                     UART_PARITY_DISABLE,
                                                     UART_STOP_BITS_1,
                                                     UART_HW_FLOWCTRL_DISABLE,
                                                     0,
                                                     UART_SCLK_DEFAULT,
                                                     0};

  static constexpr size_t UART_BUFFER_SIZE{256};
  static constexpr size_t UART_EVENT_QUEUE_SIZE{10};

  /// Construct a UART object with specified parameters.
  /// 
  /// @param[in] uartPort The UART port number (e.g., UART_NUM_0, UART_NUM_1).
  /// @param[in] txPin The GPIO pin number for UART TX.
  /// @param[in] rxPin The GPIO pin number for UART RX.
  /// @param[in] rtsPin The GPIO pin number for UART RTS (optional, default is UART_PIN_NO_CHANGE).
  /// @param[in] ctsPin The GPIO pin number for UART CTS (optional, default is UART_PIN_NO_CHANGE).
  /// @param[in] baudRate The baud rate for UART communication (default is 115200).
  /// @param[in] dataBits The number of data bits (default is UART_DATA_8_BITS).
  /// @param[in] parity The parity mode (default is UART_PARITY_DISABLE).
  /// @param[in] stopBits The number of stop bits (default is UART_STOP_BITS_1).
  /// @param[in] flowControl The hardware flow control mode (default is UART_HW_FLOWCTRL_DISABLE).
  /// @param[in] rxFlowCtrlThresh The threshold for RX flow control (default is 0).
  /// @param[in] sourceClk The source clock for UART (default is UART_SCLK_DEFAULT).
  /// @param[in] allowPd If true, allows the power domain to be powered off in sleep mode (default is false).
  ///
  /// @note This constructor initializes the UART with the specified parameters and starts it.
  UART(const uart_port_t& uartPort, 
       const int& txPin, const int& rxPin, 
       const int& rtsPin = UART_PIN_NO_CHANGE, const int& ctsPin = UART_PIN_NO_CHANGE,
       const int& baudRate = 115200, 
       const uart_word_length_t& dataBits = UART_DATA_8_BITS,
       const uart_parity_t& parity = UART_PARITY_DISABLE, 
       const uart_stop_bits_t& stopBits = UART_STOP_BITS_1,
       const uart_hw_flowcontrol_t& flowControl = UART_HW_FLOWCTRL_DISABLE,
       const uint8_t rxFlowCtrlThresh = 0,
       const uart_sclk_t& sourceClk = UART_SCLK_DEFAULT,
       const bool allowPd = false);

  /// Construct a UART object with specified parameters using a uart_config_t structure.
  ///
  /// @param[in] uartPort The UART port number (e.g., UART_NUM_0, UART_NUM_1).
  /// @param[in] txPin The GPIO pin number for UART TX.
  /// @param[in] rxPin The GPIO pin number for UART RX.
  /// @param[in] rtsPin The GPIO pin number for UART RTS (optional, default is -1).
  /// @param[in] ctsPin The GPIO pin number for UART CTS (optional, default is -1).
  /// @param[in] config The uart_config_t structure containing UART configuration parameters.
  ///
  /// @note This constructor initializes the UART with the specified configuration and starts it.
  UART(const uart_port_t& uartPort, 
       const int& txPin, const int& rxPin, 
       const int& rtsPin = -1, const int& ctsPin = -1, 
       const uart_config_t& config = DEFAULT_UART_CONFIG);

  UART(const UART&) = delete; // Disable copy constructor
  UART(UART&&) = delete; // Disable move constructor
  
  /// Destructor for the UART class. Stops the UART and deinitializes it.
  ~UART();

  UART& operator=(const UART&) = delete; // Disable copy assignment
  UART& operator=(UART&&) = delete; // Disable move assignment

private:
  /// Initialize the UART with member variables. Configures the UART port, pins, and settings.
  ///
  /// @return ESP_OK on success, or an error code on failure.
  esp_err_t init();

  /// Start the UART communication loop.
  void start();

  /// Stop the UART communication and deinitialize the UART port.
  void stop();

  uart_port_t uartPort; /// UART port number
  int txPin; /// UART TX pin number
  int rxPin; /// UART RX pin number
  int rtsPin; /// UART RTS pin number
  int ctsPin; /// UART CTS pin number
  uart_config_t uartConfig; /// UART configuration parameters

  QueueHandle_t uartQueue; /// Queue for UART events

  std::thread processingThread; /// Thread for sending and receiving data
};

#endif // __UART_H__
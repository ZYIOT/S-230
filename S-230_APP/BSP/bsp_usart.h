#ifndef __BSP_USART_H
#define __BSP_USART_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "main.h"
#include "usart.h"

#define UART1_BUFFER_SIZE       (SIZE_4K)
#define UART1_BUFFER_SIZE_MASK	(UART1_BUFFER_SIZE - 1)
#define UART1_STREAMBUF_SIZE		(SIZE_2K)

#define UART3_BUFFER_SIZE       (SIZE_1K)
#define UART3_BUFFER_SIZE_MASK	(UART3_BUFFER_SIZE - 1)
#define UART3_STREAMBUF_SIZE		(SIZE_512B)

#define UART5_BUFFER_SIZE       (UART1_BUFFER_SIZE)
#define UART5_BUFFER_SIZE_MASK	(UART5_BUFFER_SIZE - 1)
#define UART5_STREAMBUF_SIZE		(UART1_STREAMBUF_SIZE)

#define UART6_BUFFER_SIZE       (SIZE_1K)
#define UART6_BUFFER_SIZE_MASK	(UART6_BUFFER_SIZE - 1)
#define UART6_STREAMBUF_SIZE		(SIZE_512B)

#define UART4_BUFFER_SIZE       (SIZE_512B)
#define UART4_BUFFER_SIZE_MASK	(UART4_BUFFER_SIZE - 1)
#define UART4_STREAMBUF_SIZE		(SIZE_256B)

#define UART_IDEL_TICKS 300

  typedef struct
  {
    uint8_t *buffer;
    volatile size_t pos;
  } uart_buffer_t;

  extern StreamBufferHandle_t uart1_recv_buffer;
  extern StreamBufferHandle_t uart3_recv_buffer;
  extern StreamBufferHandle_t uart6_recv_buffer;
#ifdef RELAY_UART4_CAN
  extern StreamBufferHandle_t uart4_recv_buffer;
#endif  

  void BSP_UART_init(void);
  void BSP_UART_idle_irq_handler(UART_HandleTypeDef *huart);
  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
  int BSP_UART_protocol_init(void);
  int BSP_UART_protocol_send(uint32_t id, uint8_t *data, uint8_t len);
#ifdef __cplusplus
}
#endif

#endif

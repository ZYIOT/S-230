#include "string.h"
#include "main.h"
#include "usart.h"
#include "bsp_usart.h"
#include "bsp_log.h"

typedef int (*uart_process_message_fun)(uint8_t *buffer, size_t len);

uint8_t uart1_buffer[UART1_BUFFER_SIZE];
uart_buffer_t uart1_rx_buffer;
StreamBufferHandle_t uart1_recv_buffer = NULL;

uint8_t uart3_buffer[UART3_BUFFER_SIZE];
uart_buffer_t uart3_rx_buffer;
StreamBufferHandle_t uart3_recv_buffer = NULL;

uint8_t uart6_buffer[UART6_BUFFER_SIZE];
uart_buffer_t uart6_rx_buffer;
StreamBufferHandle_t uart6_recv_buffer = NULL;

#ifdef RELAY_UART4_CAN
uint8_t uart4_buffer[UART4_BUFFER_SIZE];
uart_buffer_t uart4_rx_buffer;
StreamBufferHandle_t uart4_recv_buffer = NULL;
#endif

uint8_t uart5_buffer[UART5_BUFFER_SIZE] ;
uart_buffer_t uart5_rx_buffer;

static int _send_to_uart1_recv_buffer(uint8_t *buffer, size_t len)
{
  if (len > 0)
  {
    xStreamBufferSendFromISR(uart1_recv_buffer, buffer, len, NULL);
  }
  return 0;
}

static int _send_to_uart3_recv_buffer(uint8_t *buffer, size_t len)
{
  if (len > 0)
  {
    // BSP_LOG_debug("UART3:\r\n<");
    // BSP_LOG_debug_hex(buffer, len);
    // BSP_LOG_debug(">\r\n");
    xStreamBufferSendFromISR(uart3_recv_buffer, buffer, len, NULL);
  }
  return 0;
}

#ifdef RELAY_UART4_CAN
static int _send_to_uart4_recv_buffer(uint8_t *buffer, size_t len)
{
  if (len > 0)
  {
    xStreamBufferSendFromISR(uart4_recv_buffer, buffer, len, NULL);
  }
}
#endif

static int _send_to_uart6_recv_buffer(uint8_t *buffer, size_t len)
{
  if (len > 0)
  {
    xStreamBufferSendFromISR(uart6_recv_buffer, buffer, len, NULL);
  }
  return 0;
}

static int _send_to_uart5_recv_buffer(uint8_t *buffer, size_t len)
{
#ifdef SUPPORT_UART5_CONFIG_OPTION
  if (len > 0)
  {
    xStreamBufferSendFromISR(uart1_recv_buffer, buffer, len, NULL); // 将 uart5_rx_buffer 的数据写入到 uart1_recv_buffer 中，共用一套解析操作 
  }
#endif
  return 0;
}

static int _process_receive_buffer(UART_HandleTypeDef *huart, uart_buffer_t *buffer, uart_process_message_fun callback, int dma_tc, size_t buffer_size)
{
	volatile size_t length = 0;
	volatile size_t pos = 0;
	size_t dmacnt = __HAL_DMA_GET_COUNTER(huart->hdmarx);
  size_t buffer_mask = buffer_size - 1;
	pos = buffer_size - dmacnt;
  // BSP_LOG_error("pos=%04X\tbpos=%04X\r\ndmacnt=%04X\tdma_tc=%04X\r\n", pos, buffer->pos, dmacnt, dma_tc);

	if (dma_tc)
	{
		length = buffer_size - buffer->pos;
		callback(&buffer->buffer[buffer->pos], length);
	}
	else
	{
#if 0        
		length = (pos - buffer->pos) & buffer_mask;
		if (pos >= buffer->pos)
		{
			callback(&buffer->buffer[buffer->pos], length);
		}
    else
    {
        callback(&buffer->buffer[buffer->pos], (buffer_size - buffer->pos));
        callback(&buffer->buffer[0], pos);
    }
#else
    if (pos > 0)
		{
		  length = (pos - buffer->pos) & buffer_mask;
			callback(&buffer->buffer[buffer->pos], length);
		}
#endif 
	}
	buffer->pos = pos & buffer_mask;
	return 0;
}

static void HAL_UART_process_callback(UART_HandleTypeDef *huart, int dma_ct)
{
  if (huart->Instance == USART1)
  {
    _process_receive_buffer(huart, &uart1_rx_buffer, _send_to_uart1_recv_buffer, dma_ct, UART1_BUFFER_SIZE);
  }
  else if (huart->Instance == USART3)
  {
    _process_receive_buffer(huart, &uart3_rx_buffer, _send_to_uart3_recv_buffer, dma_ct, UART3_BUFFER_SIZE);
  }
  else if (huart->Instance == USART6)
  {
    _process_receive_buffer(huart, &uart6_rx_buffer, _send_to_uart6_recv_buffer, dma_ct,UART6_BUFFER_SIZE);
  }
#ifdef RELAY_UART4_CAN
  else if (huart->Instance == UART4)
  {
    _process_receive_buffer(huart, &uart4_rx_buffer, _send_to_uart4_recv_buffer, dma_ct,UART4_BUFFER_SIZE);
  }
#endif
#ifdef SUPPORT_UART5_CONFIG_OPTION
  else if (huart->Instance == UART5)
  {
    _process_receive_buffer(huart, &uart5_rx_buffer, _send_to_uart5_recv_buffer, dma_ct, UART5_BUFFER_SIZE);
  }
#endif
}

static void _usart_dma_init(UART_HandleTypeDef *huart, uart_buffer_t *rx, uart_buffer_t *tx, uint32_t size)
{
  __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
  __HAL_UART_DISABLE_IT(huart, UART_IT_LBD);
  __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
  __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  if (rx != NULL)
  {
    rx->pos = 0;
    HAL_UART_Receive_DMA(huart, rx->buffer, size);
  }
  if (tx != NULL)
  {
    tx->pos = 0;
    HAL_UART_Transmit_DMA(huart, tx->buffer, size);
  }
  __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);
}

void BSP_UART_idle_irq_handler(UART_HandleTypeDef *huart)
{
	if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) &&
		(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) == RESET) &&
		(__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) == RESET) &&
		(__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) == RESET) &&
		(__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) == RESET)) // && (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		HAL_UART_process_callback(huart, 0);
	}
	__HAL_UART_CLEAR_IDLEFLAG(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_process_callback(huart, 1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    MX_USART1_UART_Init();
    uart1_rx_buffer.pos = 0;
    _usart_dma_init(&huart1, &uart1_rx_buffer, NULL, UART1_BUFFER_SIZE);
  }
  else if (huart->Instance == USART3)
  {
    MX_USART3_UART_Init();
    uart3_rx_buffer.pos = 0;
    _usart_dma_init(&huart3, &uart3_rx_buffer, NULL, UART3_BUFFER_SIZE);
  }
#ifdef RELAY_UART4_CAN
  else if (huart->Instance == UART4)
  {
    MX_UART4_Init();
    uart4_rx_buffer.pos = 0;
    _usart_dma_init(&huart4, &uart4_rx_buffer, NULL, UART4_BUFFER_SIZE);
  }
#endif  
  else if (huart->Instance == USART6)
  {
    MX_USART6_UART_Init();
    uart6_rx_buffer.pos = 0;
    _usart_dma_init(&huart6, &uart6_rx_buffer, NULL, UART6_BUFFER_SIZE);
  }
#ifdef SUPPORT_UART5_CONFIG_OPTION
  else if (huart->Instance == UART5)
  {
    MX_UART5_Init();
    uart5_rx_buffer.pos = 0;
    _usart_dma_init(&huart5, &uart5_rx_buffer, NULL, UART5_BUFFER_SIZE);
  }
#endif
  BSP_LOG_trace("usart reinit\r\n");
}

void BSP_UART_init(void)
{ // 【注意】：和任务堆栈一样，也是从 ucHeap[configTOTAL_HEAP_SIZE] 中动态分配
  uart1_recv_buffer = xStreamBufferCreate(UART1_STREAMBUF_SIZE, 1);
  uart1_rx_buffer.buffer = &uart1_buffer[0];
  uart1_rx_buffer.pos = 0;
  _usart_dma_init(&huart1, &uart1_rx_buffer, NULL, UART1_BUFFER_SIZE);

  uart3_recv_buffer = xStreamBufferCreate(UART3_STREAMBUF_SIZE, 1);
  uart3_rx_buffer.buffer = &uart3_buffer[0];
  uart3_rx_buffer.pos = 0;
  _usart_dma_init(&huart3, &uart3_rx_buffer, NULL, UART3_BUFFER_SIZE);

  uart6_recv_buffer = xStreamBufferCreate(UART6_STREAMBUF_SIZE, 1);
  uart6_rx_buffer.buffer = &uart6_buffer[0];
  uart6_rx_buffer.pos = 0;
  _usart_dma_init(&huart6, &uart6_rx_buffer, NULL, UART6_BUFFER_SIZE);

#ifdef RELAY_UART4_CAN
  uart4_recv_buffer = xStreamBufferCreate(UART4_STREAMBUF_SIZE, 1);
  uart4_rx_buffer.buffer = &uart4_buffer[0];
  uart4_rx_buffer.pos = 0;
  _usart_dma_init(&huart4, &uart4_rx_buffer, NULL, UART4_BUFFER_SIZE);
#endif

#ifdef SUPPORT_UART5_CONFIG_OPTION
  uart5_rx_buffer.buffer = &uart5_buffer[0];
  uart5_rx_buffer.pos = 0;
  _usart_dma_init(&huart5, &uart5_rx_buffer, NULL, UART5_BUFFER_SIZE);
#endif  
}

#ifdef RELAY_UART4_CAN
#define CAN_UART_DEVICE   (&huart4)
static HARDWARE_SEMAPHORE_TYPE_T protocol_sem;
int BSP_UART_protocol_init(void)
{
    protocol_sem = HARDWARE_CREATE_SEMAPHORE();
    return APP_OK;
}
int BSP_UART_protocol_send(uint32_t id, uint8_t *data, uint8_t len)
{
    HARDWARE_TAKE_SEMAPHORE(protocol_sem);
    uint8_t txbuff[24] = {0} ;
    write_uint32_t_BE(id, &txbuff[0]);
    txbuff[4] = len;
    for(uint8_t i=0; i<len; i++)
    {
      txbuff[5 + i] = data[i];
    }
    BSP_LOG_show("<Send>:\r\nid = %08x, dlc = %d\r\n", id, len);
    BSP_LOG_show_hex(data, len);
    BSP_LOG_show("\r\n");
    int rc = HAL_UART_Transmit(CAN_UART_DEVICE, txbuff, 5 + len, 0xffff);
    HARDWARE_GIVE_SEMAPHORE(protocol_sem);
    return rc;
}
#endif
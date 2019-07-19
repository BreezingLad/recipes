


typedef uint32_t    blk_t; /* 物理块号 */
typedef struct wfifo
{
    uint64_t    blk_size; /* 块大小 默认1G */

    uint64_t    chk_size; /* chunk大小 默认32M */

    uint64_t    fifo_head; /* fifo 首指针，生产者使用 */
    uint64_t    fifo_tail; /* fifo 尾指针，消费者使用 */
    
	uint64_t    phy_offset; /* 物理 地址偏移，第i块的物理地址： phy_offset + queue[i] * blk_size */
	
	//循环队列
    uint32_t    blk_head; /* 有效块首 */ 
    uint32_t    blk_tail; /* 有效块尾 */
    blk_t       queue[DEFAULT_RING_SIZE]; /* 块队列,可寻址空间默认为 1T */
}wfifo_t;


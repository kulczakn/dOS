#include "packets.h"

uint8_t s_socket = 0;

uint8_t s_queue_idx_first = 0;
uint8_t s_queue_idx_last = 0;
unknown_packet_t s_packet_queue[PACKET_QUEUE_LENGTH] = { 0 };
packet_callback_t s_packet_callbacks[PACKET_ID_COUNT] = { 0 };


int8_t packet_init(const char* host, uint32_t port)
{
    // local variables
    struct sockaddr_in serv_addr = { 0 };

    // create and configure socket
	if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
	{ 
		printf("\nInvalid address Address not supported \n"); 
		return -1; 
	} 

	if(connect(s_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    return 1;
}

void packet_register_callback(packet_id_t id, packet_callback_t callback)
{
    if(id < PACKET_ID_FIRST || id > PACKET_ID_LAST)
    {
        return;
    }

    s_packet_callbacks[id] = callback; 
}


void packet_enqueue(void* packet)
{
    memcpy(&s_packet_queue[s_queue_idx_last], packet, sizeof(unknown_packet_t)); 
    s_queue_idx_last = (s_queue_idx_last + 1) % PACKET_QUEUE_LENGTH;
}


void packet_dequeue(void* packet)
{
    memcpy(packet, &s_packet_queue[s_queue_idx_first], sizeof(unknown_packet_t)); 
    s_queue_idx_first = (s_queue_idx_first + 1) % PACKET_QUEUE_LENGTH;
}


void packet_close()
{
    s_socket = 0;

    // clear out queue
    memset(s_packet_queue, 0, PACKET_QUEUE_LENGTH * sizeof(unknown_packet_t));
}

// send packet from head of queue
void packet_send()
{
    unknown_packet_t packet;

    // get packet to send
    packet_dequeue(&packet);

    send(s_socket, &packet, sizeof(unknown_packet_t), 0);
}


int8_t packet_queue_length()
{
    return s_queue_idx_last - s_queue_idx_first;
}

static uint32_t con_tick_update()
{
    int valread = 0;
    uint8_t buffer[sizeof(unknown_packet_t)];
    unknown_packet_t *incoming_packet;
}

static int32_t packet_tick()
{
    /* Function description                     */
    // Client Tick:
    // - client processes incoming packets:
    //     - call handlers for each packet type
    // - client performs other processing
    //     - may or may not queue packets
    // - client sends all queued packets

    /* Local Constants                          */

    /* Local Variables                          */
    int valread = 0;
    uint8_t buffer[sizeof(unknown_packet_t)];
    unknown_packet_t *incoming_packet;

    /* Function Contents                        */
    
    while(1)
    {
        // send packets in queue
        while(packet_queue_length() > 0)
        {
            packet_send();
        }

        // check for data on socket
        valread = read(s_socket, buffer, sizeof(unknown_packet_t)); 

        // if there's data to parse
        if(valread)
        {
            incoming_packet = (unknown_packet_t *)buffer;

            // if there's a registered callback
            if(s_packet_callbacks[incoming_packet->packet_id])
            {
                s_packet_callbacks[incoming_packet->packet_id](incoming_packet);
            }
        }
    }

}


void packet_start()
{
    SDL_Thread *packet_thread;

    printf("Create packet thread:\n");

    /* Simply create a thread */
    packet_thread = SDL_CreateThread(packet_procedural, "packet_procedural", (void *)NULL);

    if (NULL == packet_thread) {
        printf("packet_thread failed: %s\n", SDL_GetError());
    }
}
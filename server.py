
import functools
import pprint
import redis
import socket
import struct
import time
import threading
import Queue
# import sqlite3 eventually use sqlite3 or more robust data storage for local data caches

from enum import Enum

# constants

class CDDPID(Enum):
    CDDP_DATA_ID_FIRST = 0,

    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST,
    CDDP_SYS_DATA_ID_LAST  = 99,

    CDDP_PRJ_DATA_ID_FIRST = 100,
    CDDP_PRJ_DATA_ID_LAST  = 199,

    CDDP_SIM_DATA_ID_FIRST = 200,
    CDDP_SIM_DATA_ID_LAST  = 299,

    CDDP_DATA_ID_LAST,

    CDDP_SYS_DATA_ID_COUNT = CDDP_SYS_DATA_ID_LAST - CDDP_SYS_DATA_ID_FIRST + 1,
    CDDP_PRJ_DATA_ID_COUNT = CDDP_PRJ_DATA_ID_LAST - CDDP_PRJ_DATA_ID_FIRST + 1,
    CDDP_SIM_DATA_ID_COUNT = CDDP_SIM_DATA_ID_LAST - CDDP_SIM_DATA_ID_FIRST + 1,
    CDDP_DATA_ID_COUNT = CDDP_DATA_ID_LAST - CDDP_DATA_ID_FIRST

# import matplotlib.pyplot as plt

class CDDPData:

    def __init__( self ):
        pass



class CDDPServer:

    def __init__( self, host, port, redis_port = 6379, conn_workers = 1, queue_workers = 1, debug = False ):
        # parameters
        self.host = host
        self.port = port

        # initialize members
        self.redis     = redis.Redis( host = "localhost", port = redis_port, db=0 )
        self.server    = None
        self.address   = None
        self.connected = False
        self.socket    = None

        self.conn_workers_count = conn_workers
        self.queue_workers_count = queue_workers

        # connections and threads
        self.server_thread      = None
        self.conn_pool          = []
        self.queue_workers_pool = []
        self.conn_workers_pool  = []

        self.input_queue  = Queue.Queue()
        self.output_queue = Queue.Queue()

        # local data
        self.data = {
            # data id -> data
        }





    # public methods

    def start( self ):
        # open socket and start server thread

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind( ( self.address, self.port ) )

        # start threads
        
        print("Starting server thread...")
        self.server_thread = threading.Thread( target = self._server_thread )
        print("Server thread started\n")


        print(f"Starting {self.conn_workers_count} connection workers...")
        for i in range(self.conn_workers_count):
            self.conn_workers_pool.append(
                threading.Thread( self._conn_worker )
            )
        print(f"{self.conn_workers_count} conn workers started\n")


        print(f"Starting {self.queue_workers_count} connection workers...")
        for i in range(self.queue_workers_count):
            self.queue_workers_pool.append(
                threading.Thread( self._queue_worker )
            )
        print(f"{self.queue_workers_count} conn workers started\n")
        

    # private methods

    def _server_thread( self ):
        # handle new connections

        while True:
            # accept new connection
            self.socket.listen( 1 )
            conn, addr = self.socket.accept()

            # add to connection pool
            self.conn_pool.append( ( conn, addr ) )


    def _conn_worker( self ):
        pass
        
        # local variables

        # until killed
        while True:

            # for each connection in connection pool
            for idx, conn, addr in enumerate(self.conn_pool):
                data = conn.recv( 128 )
                if not data continue
                else conn_alive = True

                # while there's good data and the connection is alive
                while data and conn_alive:
                    # process data
                    ddata = struct.unpack("IQ112s")

                    data_id = int(ddata[ 0 ])

                    # if it's not a DISCONN packet
                    if data_id != 4:
                        
                        # parse dat
                        data_tick = int(ddata[ 1 ])
                        data_data = bytes(ddata[ 2 ])

                        self.input_queue.put(
                            {
                                "id":   data_id,
                                "tick": data_tick,
                                "data": data_data
                            }
                        )

                        data = conn.recv( 128 )

                    else:
                        data = None
                        conn_alive = False

                # if connection is dead
                if not conn_alive:
                    self.conn_pool.remove( ( conn, addr ) )


        # end _conn_worker



    def _queue_worker( self ):
        
        # local variables

        # until killed
        while True:

            # clear the queue
            while not self.input_queue.empty():
                
                # get a packet
                packet = self.input_queue.get()

                # update local database
                self.redis.set( packet["id"], ( packet["tick"], packet["data"] ) )

                # push data to worker output queue
                


def main():
    
    server = CDDPServer(
        "localhost", 
        8989
    )

    server.start()

    while True:
        pass

    return


if __name__ == "__main__":
    main()
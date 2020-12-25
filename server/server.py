
import socket
import struct
import time
import threading
import uvicorn

# import sqlite3 eventually use sqlite3 or more robust data storage for local data caches

from enum import Enum
from fastapi import FastAPI
from queue import Queue

# constants


class CDDPData:

    def __init__( self ):
        pass



class CDDPServer:

    def __init__( self, host, port, redis_port = 6379, conn_workers = 1, queue_workers = 1, debug = False ):
        # parameters
        self.host = host
        self.port = port

        # initialize members
        # self.redis     = redis.Redis( host = "localhost", port = redis_port, db=0 )
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

        self.input_queue  = Queue()
        self.output_queue = Queue()
        self.log_queue    = Queue()

        # local data
        self.data = {
            # data id -> data
        }


    # public methods

    def start( self ):
        # open socket and start server thread

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind( ( self.host, self.port ) )

        # create threads

        print("Creating server thread...")
        self.server_thread = threading.Thread( target = self._server_thread )
        print("Server thread created\n")

        print(f"Creating {self.conn_workers_count} connection workers...")
        for i in range(self.conn_workers_count):
            conn_worker = threading.Thread( target = self._conn_worker )
            self.conn_workers_pool.append( conn_worker )
        print(f"{self.conn_workers_count} conn workers created\n")

        print(f"Creating {self.queue_workers_count} connection workers...")
        for i in range(self.queue_workers_count):
            queue_worker = threading.Thread( target = self._queue_worker )
            self.queue_workers_pool.append( queue_worker )
        print(f"{self.queue_workers_count} conn workers created\n")

        # start threads
        
        print("Starting server thread...")
        self.server_thread.start()
        print("Server thread started\n")

        print(f"Starting {self.conn_workers_count} connection workers...")
        for conn_worker in self.conn_workers_pool:
            conn_worker.start()
        print(f"{self.conn_workers_count} conn workers started\n")

        print(f"Starting {self.queue_workers_count} connection workers...")
        for queue_worker in self.queue_workers_pool:
            queue_worker.start()
        print(f"{self.queue_workers_count} conn workers started\n")

    
    def get_active( self ):
        # return list of active data ids
        pass


    def get_data( self, id ):
        # return data with given id
        pass
        

    # private methods

    def _server_thread( self ):
        # handle new connections

        while True:
            print("Server thread waiting for connection")

            # accept new connection
            self.socket.listen( 1 )
            conn, addr = self.socket.accept()

            print(f"Connection accepted from {addr}")

            # add to connection pool
            self.conn_pool.append( ( conn, addr ) )


    def _conn_worker( self ):
        pass
        
        # local variables

        # until killed
        while True:

            # for each connection in connection pool
            for idx, ( conn, addr ) in enumerate(self.conn_pool):
                data = conn.recv( 128 )
                if not data:
                    continue
                else:
                    conn_alive = True

                # while there's good data and the connection is alive
                while data and conn_alive:
                    # process data
                    ddata = struct.unpack( "IQ112s", data )

                    # TODO: LOG

                    self.log_queue.put( ddata )

                    # END TODO

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
                # self.redis.set( packet["id"], ( packet["tick"], packet["data"] ) )
                self.data[ packet[ "id" ] ] = ( packet["tick"], packet["data"] )

                # push data to worker output queue



def main():

    # local variables
    cddp_log = []
    
    # create servers
    web_server = FastAPI()
    cddp_server = CDDPServer(
        "localhost", 
        8989
    )

    # configure servers
    @web_server.get("/")
    async def root():
        return "Hello World"

    @web_server.get("/log")
    async def get_cddp_log():

        # read cddp log queue into cddp log data
        while not cddp_server.log_queue.empty():
            cddp_log.append( cddp_server.log_queue.get() )

        # return cddp log data
        return {
            "log": cddp_log
        }

    # start servers
    cddp_server.start()
    uvicorn.run(web_server, host="0.0.0.0", port=8000)

    while True:
        pass

    return


if __name__ == "__main__":
    main()
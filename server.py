
import functools
import pprint
import struct
import time
import thread

# import matplotlib.pyplot as plt


class CDDPServer:

    def __init__( self, host, port, debug = False ):
        # parameters
        self.host = host
        self.port = port

        # initialize members
        self.server = None
        self.address = None
        self.connected = False


    # public methods

    def start( self ):
        # start server thread
        print("Starting server thread...")
        thread.start_new_thread( self._server_thread )
        print("Server thread started\n")

    # private methods

    def _server_thread( self ):
        # handle new connections

        while True:
            print("Server thread pass")
            time.sleep( 10 )


def main():
    pass

if __name__ == "__main__":
    main()
import CDDP

import uvicorn
from fastapi import FastAPI


def main():
    # local variables

    # create CDDP server
    cddp_server = CDDP.Server(
        "localhost",
        8989
    )

    # run CDDP Server
    cddp_server.run()


if __name__ == "__main__":
    main()

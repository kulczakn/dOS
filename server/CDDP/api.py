from fastapi import FastAPI
from pydantic import BaseModel

class 

class API:
    def __init__(self):
        app = FastAPI()

        @app.get("/{device_addr}/{data_id}")
        def get_data(device_addr: int, data_id: int):
            pass

        @app.post("/{device_addr}/data_id}")
        def post_data(device_addr: int, data_id: int):
            pass

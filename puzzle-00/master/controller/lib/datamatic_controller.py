#!/usr/bin/etc python3
from .base_controller import BaseController

class DatamaticController(BaseController):
    def __init__(self, model, puzzle):
        super().__init__(model, puzzle)
    
    def sayHello(self):
        print("DM:")
#!/usr/bin/etc python3
from base_controller import BaseController

class DatamaticController(BaseController):
    def __init__(self, model, slave):
        super().__init__(model, slave)
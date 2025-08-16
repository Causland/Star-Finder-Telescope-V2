#!/bin/sh

python ui/quart_app.py &
python ui/dash_app.py && fg
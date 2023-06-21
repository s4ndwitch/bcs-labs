#!/bin/bash

echo $(getent passwd "$USER" | cut -d ':' -f 5)  > /tmp/stealinguser.txt
/home/daniil/Documents/University/bcs-labs/11/dist/11-1.0.0.AppImage

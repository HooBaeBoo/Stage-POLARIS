#!/bin/bash
pip3 install --user jupyter
echo 'export PATH=$PATH:~/.local/bin/' >> ~/.bashrc && . ~/.bashrc
jupyter notebook --ip=$(hostname -f)
#END

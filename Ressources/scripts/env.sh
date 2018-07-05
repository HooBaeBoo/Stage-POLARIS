#!/bin/bash
pip3 install --user jupyter
echo 'export PATH=$PATH:~/.local/bin/' >> ~/.bashrc && . ~/.bashrc
jupyter notebook --NotebookApp.iopub_data_rate_limit=10000000 --ip=$(hostname -f)
#END

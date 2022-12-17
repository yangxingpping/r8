import psutil
import time
from subprocess import PIPE
from sys import argv

if __name__ == '__main__':
    p = psutil.Popen([ argv[1] + "/plcdevice"], stdout=PIPE)
    time.sleep(5)
    print(p.is_running())
    print(p.communicate()[0])
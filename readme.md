# Install Steps #
* Log in as the pi user
* Clone `cd ~ && git clone https://github.com/the-banana-bird/rpisysfan.git && cd ~/rpisysfan`
* Build `gcc -o rpisysfan rpisysfan.c -lwiringPi`
* Service Copy `sudo cp rpisysfan.d.service /etc/systemd/system/rpisysfan.d.service`
* Service Setup `sudo systemctl enable rpisysfan.d && sudo systemctl start rpisysfan.d`

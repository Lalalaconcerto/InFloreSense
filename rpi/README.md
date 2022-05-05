# These files include all of the custom code needed to produce sonification of the plant's data
## Dependencies (to be installed on Raspberry Pi)
1. [Sonic Pi](https://sonic-pi.net)
2. [Sonic Pi Tool (Python)](https://github.com/emlyn/sonic-pi-tool)

## Steps to run:
1. Download all of these files to the Desktop of your Raspberry Pi
2. Open Sonic Pi
3. Run the `mqttToText.py` script in the `Thonny` Python IDE. Keep it running continuously.
4. Open a new terminal and run `watch -n 1 repeatedCheck.sh`

## Brief description of each script
- `infloresense.rb`: Sonification script, ran in Sonic Pi 
- `mqttToText.py`: updates `test.txt` with MQTT data
- `repeatedCheck.sh`: runs the Sonic Pi script every second
- `sonic-pi-tool.py`: copy of Sonic Pi Tool (mentioned above)
- `test.txt`: stores the most recent MQTT data

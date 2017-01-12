import yaml

with open("config/credentials.yml", 'r') as ymlfile:
    cfg = yaml.load(ymlfile)

for section in cfg:
    print section

print cfg['MCS']['device_id']

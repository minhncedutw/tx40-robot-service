import requests
import json
import numpy as np

URL = 'http://localhost:5000/v1/tx40/'
headers = {'content-type': 'application/json', 'Accept-Charset': 'UTF-8'} # config header of REST API


res = requests.get(url=URL + 'state/', headers=headers)
print(res.status_code, res.text)

data = True
res = requests.put(url=URL + 'state/', data=json.dumps(data), headers=headers)
print(res.status_code, res.text)


res = requests.get(url=URL + 'position/', headers=headers)
print(res.status_code, res.text)

data = {
    'x': -260,
    'y': 315,
    'z': 50,
    'rx': 180,
    'ry': 0,
    'rz': 0
}
res = requests.put(url=URL + 'position/', data=json.dumps(data), headers=headers)
print(res.status_code, res.text)

data = {
    'x': -260,
    'y': -75,
    'z': 50,
    'rx': 180,
    'ry': 0,
    'rz': 0
}
res = requests.put(url=URL + 'position/', data=json.dumps(data), headers=headers)
print(res.status_code, res.text)
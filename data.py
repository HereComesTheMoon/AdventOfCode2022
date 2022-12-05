import requests
from datetime import date
from configparser import ConfigParser

config = ConfigParser()
config.read('config.ini')


SESSION_ID = config['config']['SESSION_ID']
YEAR = 2022
INPUTS = "./data/"


def load(day: int) -> str:
    assert isinstance(day, int)
    assert 0 < day < 26
    s = f"https://adventofcode.com/{YEAR}/day/{day}/input"
    cookies = {"session": SESSION_ID}
    # print(cookies)

    resp: requests.Response = requests.get(s, cookies=cookies)

    return resp.text


def save(path: str, data: str):
    with open(path, 'w') as file:
        file.write(data)


if __name__ == '__main__':
    day = date.today().day
    input = load(day)
    save(f"./data/{day}.txt", input)
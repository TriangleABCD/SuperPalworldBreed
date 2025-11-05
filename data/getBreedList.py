import json
import requests
import re
import time
from itertools import combinations_with_replacement


def getBreadList(build_id, k):
    params = {
        'parent1': k,
        'parent2': "all",
        'child': "all",
    }

    url = f"https://www.palworldbreed.com/_next/data/{build_id}/zh-CN/{k}/all/all.json"

    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        "Referer": "https://www.palworldbreed.com/zh-CN/Lamball/Foxparks/all",
        "x-nextjs-data": "1"
    }

    response = requests.get(url, params=params, headers=headers)
    response.raise_for_status()

    return response.json()["pageProps"]["data"]

def getBreeds(k):
    data = getBreadList("1bQgFoCCspnvWxJYiw-UY", k)
    breeds = []
    for item in data:
        breeds.append({
            "parent2": item["parent2_pal"]["key"],
            "parent2_name": item["parent2_pal"]["name"],
            "child": item["child_pal"]["key"],
            "child_name": item["child_pal"]["name"],
        })
    return list(breeds)


if __name__ == '__main__':
    with open("pals.json", encoding="utf-8") as f:
        pals = json.load(f)

    results = []
    for pal in pals:
        print(f"查询 {pal['id']}-{pal['name']}({pal['key']})的所有子代...")
        key = pal["key"]
        try:
            breeds = getBreeds(key)
            for breed in breeds:
                print(f"{pal['name']} x {breed['parent2_name']} -> {breed['child_name']}")
                results.append({
                    "parent1": key,
                    "parent2": breed["parent2"],
                    "child": breed["child"],
                })
        except Exception as e:
            print(" !! 失败，跳过：", e)
        time.sleep(0.3)

    print(f"共查询到{len(results)}条配种结果")
    with open("breed.json", "w", encoding="utf-8") as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    print("全部完成，已保存 breed.json")

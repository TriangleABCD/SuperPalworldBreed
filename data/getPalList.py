import re
import json
import requests

def getPalList(build_id):
    params = {
        'parent1': "all",
        'parent2': "all",
        'child': "all",
    }

    url = f"https://www.palworldbreed.com/_next/data/{build_id}/zh-CN/all/all/all.json"

    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        "Referer": "https://www.palworldbreed.com/zh-CN/Lamball/Foxparks/all",
        "x-nextjs-data": "1"
    }

    response = requests.get(url, params=params, headers=headers)
    response.raise_for_status()

    return response.json()["pageProps"]["pals"]


if __name__ == '__main__':
    data = getPalList("1bQgFoCCspnvWxJYiw-UY")
    palList = [{
        "id": str(item["number"]),
        "name": item["name"],
        "key": item["key"],
    } for item in data]

    out_file = "pals.json"
    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(palList, f, ensure_ascii=False, indent=2)

    print(f"已提取 {len(palList)} 条记录，保存为 {out_file}")

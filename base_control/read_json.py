import pandas as pd
from os import listdir
from os.path import isfile, join


def json2csv(json_data, run):
    if run == 2:
        df = pd.DataFrame([json_data], columns=list(json_data.keys()))
        df.to_csv("output.csv", index=False)
    if run > 2:
        df = pd.DataFrame([json_data], columns=list(json_data.keys()))
        df.to_csv("output.csv", header=None, index=False, mode="a")


def read_json(f):
    global run
    run = run + 1

    file_ = join(path, f)

    test_json = pd.read_json(file_, orient="records")
    json_geometry = test_json["geometry"]
    json_properties = test_json["properties"]

    take_data = lambda s1, s2: s1 if str(s2) == "nan" else s2
    print(json_properties.combine(json_geometry, take_data))

    print("---------------")
    print(json_properties)
    print("---------------")
    print(json_geometry)


if __name__ == "__main__":
    global path
    global endswithjson
    global run
    path = "./json_files"
    endswithjson = ".geojson"

    run = 1
    onlyfiles = sorted(
        [f for f in listdir(path) if isfile(join(path, f)) and f.endswith(endswithjson)]
    )
    for f in onlyfiles:
        read_json(f)

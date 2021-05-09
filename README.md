# GAP
Game Analytics Exporter

Built using Visual Studio 2019

## Brief

This project generates heatmaps based on ElasticSearch database and Amazon Web Services. The parser searches for data entries with the next fields

* event_id : Can be either "world" or "screen"
* x : x position 
* y : y position
* z : z position

## Dependencies

Create a Folder with the next Dependency projects

* [SFML 2.5](https://www.sfml-dev.org/download/sfml/2.5.1/)
* [Amazon Web Services SDK C++](https://github.com/aws/aws-sdk-cpp)
* [libcurl](https://curl.se/windows/)
* [gzip hpp](https://github.com/mapbox/gzip-hpp)

Create libraries for the needed version you are using (Debug/Release or x64/x86)

After that, create an Environment Variable called DEVLIBS

dlls are needed for the executable to run

## Installation

```
./include/
$(DEVLIBS)/rapidjson/include/
$(DEVLIBS)/gzip-hpp/include/
$(DEVLIBS)/SFML/2.5/include
$(DEVLIBS)/aws-sdk-cpp/aws-cpp-sdk-s3/include/
$(DEVLIBS)/aws-sdk-cpp/aws-cpp-sdk-core/include/
$(DEVLIBS)/curl/include/


./lib/$(PlatformTarget)/$(Configuration)/
$(DEVLIBS)/SFML/2.5/$(PlatformTarget)/lib/$(Configuration)/
$(DEVLIBS)/aws-sdk-cpp/$(PlatformTarget)/bin/$(Configuration)/
$(DEVLIBS)/aws-sdk-cpp/$(PlatformTarget)/aws-cpp-sdk-s3/$(Configuration)/
$(DEVLIBS)/aws-sdk-cpp/$(PlatformTarget)/aws-cpp-sdk-core/$(Configuration)/
$(DEVLIBS)/curl/builds/$(PlatformTarget)/$(Configuration)/lib/

```

Add the .dll files in the same folder as the executable.


## Configuration

The GAP Heatmap generator depends on a json file called 

> script.json

This file is loaded constantly in the parser, loading configuration settings which will define how the parser behaves

### database

Type: String

Database defines the database to use. This is not being used right now, but was designed so when changing databases, it could be easily implemented a new database with inheritance.

By default, the database


> "database":"SE"

### url

Type: String

This is the bucket where the Parser will get the information from

> "url":"https://master:yceKIBANA2020!@search-yce-es-dev-6gr5xzmxq2vztgixcidkfafzry.us-west-1.es.amazonaws.com"

### index

Type: String

The database to get information from. In ES, this is called type. 

> "index":"oset"

### timestoparse

Type: Int

How many times it will traverse all the queries

> "timestoparse":3

### sizeperparse

Type: Int

How many information per query will store. In Elastic search the max is 1000

> "sizeperparse":1000

### timedifference

Type: Int

This is not implemented, but intended so you can sort things with a max time

> "timedifference":1800

### bucket

Type: String

The bucket to store the images generated. This is a bucket from AWS

> "bucket": "release.oset"

### folder

Type: String

The folder inside the bucket to store the images.

> "folder": "content"

### region

Type: String

The region that aws asks for your account. By default is us-west-1

> "region": "us-west-1"

### texturewidth

Type: Int

The max size of the texture in width the image generated will have

> "texturewidth": 1024

### textureheight

Type: Int

The max size the texture in height will have

> "textureheight": 768

### momentum

Type: Float

Heatmap decay

> "momentum": 0.8

### decay

Type: Float

> "decay": 0.035

### hottest

Type: Float Array

> "hottest": [255,71,71,0.75]

### hot

Type: Float Array

> "hot": [255,0,169,0.5]

### cold

Type: Float Array

> "cold": [60,88,191,0.75]

### coldest

Type: Float Array

> "coldest": [121,255,223,0.5]

### minpoint

Type: Float Array

> "minpoint": [-10610,-10080,-130]

### maxpoint

Type: Float Array

> "maxpoint": [9800,16880,1850]

## Classes
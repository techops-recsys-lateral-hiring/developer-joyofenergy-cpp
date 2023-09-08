### The main branch now adopts Conan v2.x, which changed the building steps quite a lot, you could switch to `legacy` branch for Conan v1.x

[![CI](https://github.com/techops-recsys-lateral-hiring/developer-joyofenergy-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/techops-recsys-lateral-hiring/developer-joyofenergy-cpp/actions/workflows/ci.yml)

# Welcome to PowerDale

PowerDale is a small town with around 100 residents. Most houses have a smart meter installed that can save and send
information about how much power a house is drawing/using.

There are three major providers of energy in town that charge different amounts for the power they supply.

- _Dr Evil's Dark Energy_
- _The Green Eco_
- _Power for Everyone_

# Introducing JOI Energy

JOI Energy is a new start-up in the energy industry. Rather than selling energy they want to differentiate themselves
from the market by recording their customers' energy usage from their smart meters and recommending the best supplier to
meet their needs.

You have been placed into their development team, whose current goal is to produce an API which their customers and
smart meters will interact with.

Unfortunately, two members of the team are on annual leave, and another one has called in sick! You are left with
another ThoughtWorker to progress with the current user stories on the story wall. This is your chance to make an impact
on the business, improve the code base and deliver value.

## Story Wall

At JOI energy the development team use a story wall or Kanban board to keep track of features or "stories" as they are
worked on.

The wall you will be working from today has 7 columns:

- Backlog
- Ready for Dev
- In Dev
- Ready for Testing
- In Testing
- Ready for sign off
- Done

Examples can be found
here [https://leankit.com/learn/kanban/kanban-board/](https://leankit.com/learn/kanban/kanban-board/)

## Users

To trial the new JOI software 5 people from the JOI accounts team have agreed to test the service and share their energy
data.

| User    | Smart Meter ID  | Power Supplier        |
| ------- | --------------- | --------------------- |
| Sarah   | `smart-meter-0` | Dr Evil's Dark Energy |
| Peter   | `smart-meter-1` | The Green Eco         |
| Charlie | `smart-meter-2` | Dr Evil's Dark Energy |
| Andrea  | `smart-meter-3` | Power for Everyone    |
| Alex    | `smart-meter-4` | The Green Eco         |

These values are used in the code and in the following examples too.

## Requirements

1. CMake 3.23 above is **required**.

Check [here](https://cmake.org/download/) for installation if you get a older version.

2. Conan 2.0 above is **required**.

Check [here](https://docs.conan.io/2/installation.html) for installation if you get a older version.

The project makes use of Conan to help you out carrying some common tasks such as building the project or running it.

Technecally, Conan is not must have, but in order to simplify environment setup, following guide line is base on Conan.

### Build the project

0. Setup a profile for Conan, if this is the first time you get Conan 2.x run on your machine.

```shell
$ conan profile detect
```

Now, assuming you are already at the root of this repository, follow:

1. Install or build the dependencies


```shell
$ conan install . --output-folder=build -s build_type=Debug --build=missing
```

2. Generate build system for this repository

```shell
$ cmake --preset conan-debug
```

> ☀️ If you are building on **Windows with Visual Studio** (Generator), please use `conan-default` instead of, like:
>
> ```shell
> $ cmake --preset conan-default
> ```

3. Build this repository

```shell
$ cmake --build .
```

> ☀️ If you are building on **Windows with Visual Studio** (Generator), and prefer IDE, you could find a generate project file in `build` directory and open it with Visual Studio for building and test running.

> ⚡️With option `-j<number>` (e.g. -j4 ) could enable parallel build job, which could usually accelerate your build.

### ‼️ Build Troubleshooting

If you suffer compile error like this during build step (especial on Linux with GCC):

> undefined reference to `testing::Message::GetString\[abi:cxx11]() const'

It's a well-known issue of GCC switching C++ runtime library from `libstdc++` to `libstdc++11` which are ABI incompatible. The most of earier versions of conan might design to remain using `libstdc++` for back
ard compatibility, to tweak conan to adopt `libstdc++11` explictly by using option `-s compiler.libcxx=libstdc++11`, you can change the second command above to:

```console
$ conan install .. -s compiler.libcxx=libstdc++11 --build missing
```

Then proceed with subsequent commands and it should build fine.

### Run the tests

There are two types of tests, the unit tests and the functional tests. These can be executed as follows.

- Run functional tests only

  ```shell
  $ ctest .
  ```


### Run the application
  ```shell
  $ ./app <address> <kPort> <concurrency>
  ```

## API

Below is a list of API endpoints with their respective input and output. Please note that the application needs to be
running for the following endpoints to work. For more information about how to run the application, please refer
to [run the application](#run-the-application) section above.

### Store Readings

Endpoint

```text
POST /readings/store
```

Example of body

```json
{
  "smartMeterId": <smartMeterId>,
  "electricityReadings": [
    {
      "time": <time>,
      "reading": <reading>
    }
  ]
}
```

Parameters

| Parameter      | Description                                           |
| -------------- | ----------------------------------------------------- |
| `smartMeterId` | One of the smart meters' id listed above              |
| `time`         | The date/time (as RFC3399) when the _reading_ was taken |
| `reading`      | The consumption in `kW` at the _time_ of the reading  |

Example readings

| Date (`GMT`)      | RFC3399                       | Reading (`kW`) |
| ----------------- | ----------------------------- | -------------: |
| `2020-11-11 8:00` | `2020-11-18T08:00:00.725202Z` |         0.0503 |
| `2020-11-12 8:00` | `2020-11-18T08:00:00.725202Z` |         0.0213 |

In the above example, the smart meter sampled readings, in `kW`, every minute. Note that the reading is in `kW` and
not `kWH`, which means that each reading represents the consumption at the reading time. If no power is being consumed
at the time of reading, then the reading value will be `0`. Given that `0` may introduce new challenges, we can assume
that there is always some consumption, and we will never have a `0` reading value. These readings are then sent by the
smart meter to the application using REST. There is a service in the application that calculates the `kWH` from these
readings.

The following POST request, is an example request using CURL, sends the readings shown in the table above.

```shell
$ curl \
  -X POST \
  -H "Content-Type: application/json" \
  "http://localhost:8080/readings/store" \
  -d '{"smartMeterId":"smart-meter-0","electricityReadings":[{"time":"2020-11-18T08:00:00.725202Z","reading":0.0503},{"time":"2020-11-18T08:00:00.725202Z","reading":0.0213}]}'
```

The above command does not return anything.

### Get Stored Readings

Endpoint

```text
GET /readings/read/<smartMeterId>
```

Parameters

| Parameter      | Description                              |
| -------------- | ---------------------------------------- |
| `smartMeterId` | One of the smart meters' id listed above |

Retrieving readings using CURL

```shell
$ curl "http://localhost:8080/readings/read/smart-meter-0"
```

Example output

```json
{
    "readings": [
        {
            "reading": 0.4,
            "time": "2021-08-20T07:04:49.000000Z"
        },
        {
            "reading": 0.123,
            "time": "2021-08-20T07:07:49.000000Z"
        },
        {
            "reading": 0.23,
            "time": "2021-08-20T07:10:49.000000Z"
        },
        {
            "reading": 0.432,
            "time": "2021-08-20T07:13:49.000000Z"
        },
        {
            "reading": 0.44,
            "time": "2021-08-20T07:16:49.000000Z"
        }
    ]
}
```

### View Current Price Plan and Compare Usage Cost Against all Price Plans

Endpoint

```text
GET /price-plans/compare-all/<smartMeterId>
```

Parameters

| Parameter      | Description                              |
| -------------- | ---------------------------------------- |
| `smartMeterId` | One of the smart meters' id listed above |

Retrieving readings using CURL

```shell
$ curl "http://localhost:8080/price-plans/compare-all/smart-meter-0"
```

Example output

```json
{
    "pricePlanComparisons": {
        "price-plan-0": 4.0,
        "price-plan-1": 0.8,
        "price-plan-2": 0.4
    },
    "pricePlanId": "price-plan-1"
}
```

### View Recommended Price Plans for Usage

Endpoint

```text
GET /price-plans/recommend/<smartMeterId>[?limit=<limit>]
```

Parameters

| Parameter      | Description                                          |
| -------------- | ---------------------------------------------------- |
| `smartMeterId` | One of the smart meters' id listed above             |
| `limit`        | (Optional) limit the number of plans to be displayed |

Retrieving readings using CURL

```shell
$ curl "http://localhost:8080/price-plans/recommend/smart-meter-0?limit=2"
```

Example output

```json
{
    "recommend": [
        {
            "price-plan-2": 0.4
        },
        {
            "price-plan-1": 0.8
        }
    ]
}
```

# ESP32-DATAHUB
Code from the blog article on ESP32 and LoRaWAN.
Link to the article: To be provided when published.

## Prerequisites
In order to be able to configure the DataHub policy, you need:

- The HiveMQ API enabled on the broker
- The [HiveMQ MQTT CLI](https://github.com/hivemq/hivemq-mqtt-client) (latest version)


## Upload the schema(s)
```
mqtt hivemq schema create --id pre-lorawan-application --type json --file pre-schema.json
mqtt hivemq schema create --id post-lorawan-application --type json --file post-schema.json
```

## Upload the script
```
mqtt hivemq script create --id lorawan-transformation --type transformation --file script.js
```

## Upload the policy
```
mqtt hivemq data-policy create --file policy.json
```

## Run Arduino code
The Arduino code has been created from a sample provided on SeeedStudio Wiki and modified to fit our requirments.

It will run on the following setup:
- [XIAO ESP32S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html?queryID=223cfe91073c04ae77a3ab037842274e&objectID=5627&indexName=bazaar_retailer_products)
- [Grove Shield for XIAO ESP32](https://www.seeedstudio.com/Grove-Shield-for-Seeeduino-XIAO-p-4621.html?queryID=b3f8cf140aa111420dc069bfb28b7e90&objectID=4621&indexName=bazaar_retailer_products)
- [Grove-Wio-E5 Wireless Module](https://www.seeedstudio.com/Grove-LoRa-E5-STM32WLE5JC-p-4867.html?queryID=3ddc93179e36e45e456f86f18ad5060e&objectID=4867&indexName=bazaar_retailer_products) (868 Mhz for EU)
- [Grove - Temperature & Humidity Sensor (DHT11)](https://www.seeedstudio.com/Grove-Temperature-Humidity-Sensor-DHT11.html?queryID=ce282511cefd6b77ac6bc4307b65659d&objectID=1826&indexName=bazaar_retailer_products)
- [Grove cables (in case you don’t have enought)](https://www.seeedstudio.com/Grove-Universal-4-Pin-20cm-Unbuckled-Cable-5-PCs-Pack-p-749.html?queryID=a2713b3fa73e30bf90288d0ff171aaf3&objectID=1822&indexName=bazaar_retailer_products)

Follow the setups in the article to wire everything together and deploy the code on it.
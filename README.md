# ESP32-DATAHUB
Code from the blog article on ESP32 and LoRaWAN.


## Upload the schema 
```
mqtt hivemq schema create --id schema-lorawan-application --type json --file schema.json
```

## Upload the script
```
mqtt hivemq script create --id lorawan-transformation --type transformation --file script.js
```
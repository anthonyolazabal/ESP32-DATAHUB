function decodeBase64(base64String) {
    const base64Chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
    let base64 = base64String.replace(/=+$/, '');
    let binaryString = '';

    for (let i = 0; i < base64.length; i++) {
        let binaryChar = base64Chars.indexOf(base64[i]).toString(2);
        binaryChar = binaryChar.padStart(6, '0');
        binaryString += binaryChar;
    }

    let decodedString = '';
    for (let i = 0; i < binaryString.length; i += 8) {
        let byte = binaryString.substring(i, i + 8);
        decodedString += String.fromCharCode(parseInt(byte, 2));
    }

    return decodedString;
}

function transform(publish, context) {
    console.log(publish.payload.data)
    let values = decodeBase64(publish.payload.data).split('-')
    let temperature = values[0].split(":")[1]
    let humidity = values[1].split(":")[1]

    const newPayload = {
        topic: publish.topic,
        payload: {
            "temperature": temperature,
            "humidity": humidity,
            "deviceName": publish.payload.deviceInfo.deviceName,
            "time": publish.payload.time
        },
        userProperties: [{ name: "transformed", value: "true" }]
    }
    return newPayload;
}
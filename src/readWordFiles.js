const mammoth = require('mammoth');
const path = require('path');
const fs = require('fs');

/**
 * https://www.npmjs.com/package/mammoth
 * 不支持公式识别
 */
mammoth.extractRawText({path: path.join(__dirname, '../test.docx')}, {
    convertImage: mammoth.images.imgElement(function(image) {
        return image.read("base64").then(function(imageBuffer) {
            return {
                src: "data:" + image.contentType + ";base64," + imageBuffer
            };
        });
    })
})
 .then(text => {
    console.log(text.value)
    fs.writeFileSync(path.join(__dirname, '../dist/out.html'), text.value);
})
 .catch(error => console.log(error));

// Timeout in milliseconds for API call
var timeout = 10000;

// underComm = 0 : No communication state, >= 1 : Communication state
var underComm = 0;

// [0] : for API-1, [1] : for API-2, [2] : for API-3, ...
var statusCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var responseData = [{},{},{},{},{},{},{},{},{},{},{},{},{},{}];


function sendRequestRecvResponse(url, request, response) {
    var resultcode = 0;
    $.ajax({
        type: 'GET',
        dataType: 'json',
        url: url,
        data: request,
        async: false,
        success: function (msg, textStatus, xhr) {
            resultcode = xhr.status;
            $.extend(response, msg);
        },
        error: function () {
            resultcode = -1;
        }
    });
    return resultcode;
}

function apiCall(method, url, request, index, targetFunc) {
    sendRequestRecvResponseA(method, url, request, index);
    waitForResponse(index, 0, targetFunc);
}

function waitForResponse(index, count, targetFunc) {
    if (count < 10) {
        if (underComm == 0) {
            targetFunc(index);
        } else {
            setTimeout(function() {waitForResponse(index, count + 1, targetFunc);}, 50);
        }
        return;
    }
    if (underComm == 0) {
        $('#loading_Modal').modal('hide');
        targetFunc(index);
        return;
    }
    $('#loading_Modal').modal('show');
    setTimeout(function() {waitForResponse(index, count + 1, targetFunc);}, 500);
}

function sendRequestRecvResponseA(method, url, request, index) {
    underComm++;
    if (method === 'GET') {
        $.ajax({
            type: method,
            dataType: 'json',
            url: url,
            data: request,
            async: true,
            timeout: timeout,
            success: function(msg, textStatus, xhr) {
                statusCode[index] = xhr.status;
                $.extend(responseData[index], msg);
                underComm--;
            },
            error: function(xhr, textStatus, errorThrown) {
                if (textStatus === 'timeout') {
                    statusCode[index] = -1;
                } else {
                    statusCode[index] = xhr.status;
                    $.extend(responseData[index], JSON.parse(xhr.responseText));
                }
                underComm--;
            }
        });
    } else {
        $.ajax({
            type: method,
            dataType: 'json',
            url: url,
            data: JSON.stringify(request),
            contentType: 'application/json',
            async: true,
            timeout: timeout,
            success: function(msg, textStatus, xhr) {
                statusCode[index] = xhr.status;
                $.extend(responseData[index], msg);
                underComm--;
            },
            error: function(xhr, textStatus, errorThrown) {
                if (textStatus === 'timeout') {
                    statusCode[index] = -1;
                } else {
                    statusCode[index] = xhr.status;
                    $.extend(responseData[index], JSON.parse(xhr.responseText));
                }
                underComm--;
            }
        });
    }
    return;
}

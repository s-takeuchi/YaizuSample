
// Timeout in milliseconds for API call
var timeout = 10000;

// underComm = 0 : No communication state, >= 1 : Communication state
var underComm = 0;

// [0] : for API-1, [1] : for API-2, [2] : for API-3, ...
var statusCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var responseData = [{},{},{},{},{},{},{},{},{},{},{},{},{},{}];


function apiCall(method, url, request, index, targetFunc) {
    sendRequestRecvResponse(method, url, request, index);
    setTimeout(function() {waitForResponse(index, 0, targetFunc);}, 1);
}

function waitForResponse(index, count, targetFunc) {
    if (targetFunc == null) {
        return;
    }
    if (count < 10) {
        if (underComm == 0) {
            targetFunc();
        } else {
            setTimeout(function() {waitForResponse(index, count + 1, targetFunc);}, 50);
        }
        return;
    }
    if (underComm == 0) {
        $('#loading_Modal').modal('hide');
        targetFunc();
        return;
    }
    $('#loading_Modal').modal('show');
    setTimeout(function() {waitForResponse(index, count + 1, targetFunc);}, 500);
}

function sendRequestRecvResponse(method, url, request, index) {
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

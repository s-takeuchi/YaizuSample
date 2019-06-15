
// Timeout in milliseconds for API call
var timeout = 10000;

// underComm = 0 : No communication state, >= 1 : Communication state
var underComm = 0;

// [0] : for API-1, [1] : for API-2, [2] : for API-3, ...
var statusCode = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var responseData = [{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}];

// Init flag of loading modal
var initLoadingModalFlag = false;

// Client messages
var clientMsg = {};
var clientLanguage = 0;

function initLoadingModal() {
    var loadingModal = $('<div id="loading_Modal" class="modal fade" tabindex="-1" role="dialog" data-keyboard="false" data-backdrop="static">');
    var modalDialog = $('<div class="modal-dialog">');
    var modalContent = $('<div class="modal-content">');
    var modalHeader = $('<div class="modal-header" style="padding: 0px; text-align: center;"><h3 style="margin-top: 5px;">Now loading ...</h3></div>');
    var modalBody = $('<div class="modal-body"><div style="height:20px"><div class="progress"><div class="progress-bar progress-bar-striped active" role="progressbar" aria-valuenow="40" aria-valuemin="0" aria-valuemax="100" style="width:100%"> </div></div></div></div>');

    modalContent.append(modalHeader);
    modalContent.append(modalBody);
    modalDialog.append(modalContent);
    loadingModal.append(modalDialog);
    $('body').append(loadingModal);
}

function addClientMessage(code, msg) {
    if (msg.en !== undefined && msg.ja !== undefined) {
        clientMsg[code] = msg;
    }
}

function deleteClientMessage(code) {
    delete clientMsg[code];
}

function getClientMessageEn(code) {
    return clientMsg[code].en;
}

function getClientMessageJa(code) {
    return clientMsg[code].ja;
}

function setClientLanguage(lang) {
    clientLanguage = lang;
}

function getClientMessage(code) {
    if (clientLanguage == 0) {
        return clientMsg[code].en;
    }
    if (clientLanguage == 1) {
        return clientMsg[code].ja;
    }
}

function displayAlertSuccess(parent, msgCode) {
    $(parent).append('<div class="alert alert-success" role="alert">' + getClientMessage(msgCode) + '</div>');
}

function displayAlertDanger(parent, msgCode) {
    $(parent).append('<div class="alert alert-danger" role="alert">' + getClientMessage(msgCode) + '</div>');
}

function displayAlertWarning(parent, msgCode) {
    $(parent).append('<div class="alert alert-warning" role="alert">' + getClientMessage(msgCode) + '</div>');
}

function displayAlertInfo(parent, msgCode) {
    $(parent).append('<div class="alert alert-info" role="alert">' + getClientMessage(msgCode) + '</div>');
}

function apiCall(method, url, request, index, targetFunc) {
    if (method != null && url != null && index != -1) {
        sendRequestRecvResponse(method, url, request, index);
    }
    if (targetFunc != null) {
        setTimeout(function() {waitForResponse(0, targetFunc);}, 1);
    }
}

function waitForResponse(count, targetFunc) {
    if (initLoadingModalFlag == false) {
        initLoadingModalFlag = true;
        initLoadingModal();
    }
    if (targetFunc == null) {
        return;
    }
    if (count < 10) {
        if (underComm == 0) {
            targetFunc();
        } else {
            setTimeout(function() {waitForResponse(count + 1, targetFunc);}, 50);
        }
        return;
    }
    if (underComm == 0) {
        $('#loading_Modal').modal('hide');
        targetFunc();
        return;
    }
    $('#loading_Modal').modal('show');
    setTimeout(function() {waitForResponse(count + 1, targetFunc);}, 500);
}

function sendRequestRecvResponse(method, url, request, index) {
    underComm++;
    if (method === 'GET' || method === 'DELETE') {
        $.ajax({
            type: method,
            dataType: 'json',
            url: url,
            data: request,
            async: true,
            timeout: timeout,
            success: function(msg, textStatus, xhr) {
                statusCode[index] = xhr.status;
                responseData[index] = {};
                $.extend(responseData[index], msg);
                underComm--;
            },
            error: function(xhr, textStatus, errorThrown) {
                if (textStatus === 'timeout') {
                    statusCode[index] = -1;
                    responseData[index] = {};
                } else {
                    statusCode[index] = xhr.status;
                    try {
                        $.extend(responseData[index], JSON.parse(xhr.responseText));
                    } catch (e) {
                        responseData[index] = {};
                    }
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
                responseData[index] = {};
                $.extend(responseData[index], msg);
                underComm--;
            },
            error: function(xhr, textStatus, errorThrown) {
                if (textStatus === 'timeout') {
                    statusCode[index] = -1;
                    responseData[index] = {};
                } else {
                    statusCode[index] = xhr.status;
                    try {
                        $.extend(responseData[index], JSON.parse(xhr.responseText));
                    } catch (e) {
                        responseData[index] = {};
                    }
                }
                underComm--;
            }
        });
    }
    return;
}

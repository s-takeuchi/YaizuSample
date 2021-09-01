
// User role
var userRole = 0;

// Intervals
var selectedPollingInterval = 1;
var pollingIntervalStr = ['30 sec ', '1 min ', '5 min ', '15 min '];
var selectedStatusAcquisitionInterval = 2;
var statusAcquisitionIntervalStr = ['5 min ', '15 min ', '30 min ', '60 min '];

// Selected command ID on command page
var selectedCommand = -1;

// Selected user on user management page
var selectedUser = '';

// Selected index number of status acquisition command on agent info page.
var selectedAgentStatusCommand = -1;
// Selected index number of operation command on agent info page.
var selectedOperationCommand = -1;

// Notifications
var svrinfo_msg = '';
var command_msg = '';
var usermgt_msg = '';

function initClientMessage() {
    addClientMessage('USER_MGMT', {'en':'User Management', 'ja':'ユーザー管理'});
    addClientMessage('USER_CHG_PW', {'en':'Change Password', 'ja':'パスワードの変更'});
    addClientMessage('OPE_LOG', {'en':'Operation log', 'ja':'操作ログ'});
    addClientMessage('SERVER_INFO', {'en':'Server info', 'ja':'サーバ情報'});

    addClientMessage('AGENTINFO', {'en':' Agent Info', 'ja':' Agent Info'});
    addClientMessage('SERVERINFO', {'en':' Server Info', 'ja':' Server Info'});
    addClientMessage('COMMAND', {'en':' Command', 'ja':' Command'});

    addClientMessage('NOAGTINFO', {'en':'<p>No agent information</p>', 'ja':'<p>エージェント情報はありません</p>'});
    addClientMessage('NOCMDEXIST', {'en':'<p>No command exists</p>', 'ja':'<p>コマンドはありません</p>'});
    addClientMessage('NOFILEEXIST', {'en':'<p>No file exists</p>', 'ja':'<p>ファイルはありません</p>'});
    addClientMessage('NORESULTEXIST', {'en':'<p>No command result exists</p>', 'ja':'<p>コマンド実行結果はありません</p>'});

    addClientMessage('DASHBOARD_AGENTS', {'en':'Agent status', 'ja':'エージェントの状態'});
    addClientMessage('DASHBOARD_SELECt_AGENT', {'en':'Select agent', 'ja':'エージェントの選択'});
    addClientMessage('DASHBOARD_STATUS_DESC', {'en':'You can choose maximum 8 agents time series status on dashboard.', 'ja':'最大8件のエージェントの状態をダッシュボードに表示できます。'});
    addClientMessage('DASHBOARD_UNSPECIFIED', {'en':'Unspecified', 'ja':'未指定'});

    addClientMessage('AINAME', {'en':'Name', 'ja':'名称'});
    addClientMessage('AISTATUS', {'en':'Status', 'ja':'状態'});
    addClientMessage('AISTATUSCMD', {'en':'Command for Status', 'ja':'状態取得コマンド'});
    addClientMessage('AISTATUSINITIME', {'en':'Initial Notification Time', 'ja':'初回通知時刻'});
    addClientMessage('AILASTPOLLINGTIME', {'en':'Last Polling Time', 'ja':'最終ポーリング時刻'});
    addClientMessage('AISTATUSTIME', {'en':'Status Acquisition Time', 'ja':'状態取得時刻'});
    addClientMessage('AISTATUSUPTIME', {'en':'Status Update Time', 'ja':'状態更新時刻'});
    addClientMessage('AIOPSTATUS', {'en':'OpStatus', 'ja':'操作状態'});
    addClientMessage('AIOPSTATUSCMD', {'en':'Command for Op', 'ja':'操作コマンド'});
    addClientMessage('AISETSTATUSCMD', {'en':'Select command', 'ja':'コマンド設定'});
    addClientMessage('AIEXECCMD', {'en':'Execute command', 'ja':'コマンド実行'});
    addClientMessage('SELOPCMD', {'en':'Select operation command', 'ja':'操作コマンドを選択してください'});
    addClientMessage('SELSTATUSCMD', {'en':'Select status acquisition command', 'ja':'状態取得コマンドを選択してください'});
    addClientMessage('SELCMD', {'en':'Select Command ', 'ja':'コマンドを選択してください'});
    addClientMessage('RESCODE-980', {'en':'-980 : Agent service has started.\r\n', 'ja':'-980 : エージェントサービスが起動した\r\n'});
    addClientMessage('RESCODE-981', {'en':'-981 : No script is defined.\r\n', 'ja':'-981 : スクリプトが定義されていない\r\n'});
    addClientMessage('RESCODE-982', {'en':'-982 : Status acquisition cmd has been changed.\r\n', 'ja':'-982 : 状態取得コマンドが変更された\r\n'});
    addClientMessage('RESCODE-983', {'en':'-983 : Operation cmd has been changed.\r\n', 'ja':'-983 : 操作コマンドが変更された\r\n'});
    addClientMessage('RESCODE-984', {'en':'-984 : Waiting for a request.\r\n', 'ja':'-984 : 要求待ち\r\n'});
    addClientMessage('RESCODE-985', {'en':'-985 : A command for operation has started.\r\n', 'ja':'-985 : 操作コマンドが実行された\r\n'});
    addClientMessage('RESCODE-990', {'en':'-990 : Server file handling error.\r\n', 'ja':'-990 : サーバファイルハンドリングエラー\r\n'});
    addClientMessage('RESCODE-991', {'en':'-991 : Agent file handling error.\r\n', 'ja':'-991 : エージェントファイルハンドリングエラー\r\n'});
    addClientMessage('RESCODE-992', {'en':'-992 : Platform error.\r\n', 'ja':'-992 : プラットフォームエラー\r\n'});
    addClientMessage('RESCODE-993', {'en':'-993 : No request from agent.\r\n', 'ja':'-993 : エージェントからの要求がない\r\n'});
    addClientMessage('RESCODE-994', {'en':'-994 : Invalid agent directory.\r\n', 'ja':'-994 : エージェントディレクトリが不正\r\n'});
    addClientMessage('RESCODE-995', {'en':'-995 : Command execution result handling error.\r\n', 'ja' : '-995 : コマンド実行結果ハンドリングエラー\r\n'});

    addClientMessage('SISTARTTIME', {'en':'Service start time : ', 'ja':'サービス開始時刻 : '});
    addClientMessage('SISVRVERSION', {'en':'Server version : ', 'ja':'サーバーバージョン : '});
    addClientMessage('SIPOLLINTVL', {'en':'Polling interval : ', 'ja':'ポーリング間隔 : '});
    addClientMessage('SISTACQINTVL', {'en':'Status acquisition interval : ', 'ja':'状態取得間隔 : '});
    addClientMessage('SIUPDATEBTN', {'en':'Update', 'ja':'更新'});
    addClientMessage('SIUPDATED', {'en':'The server information has been updated.', 'ja':'サーバー情報が更新されました。'});
    addClientMessage('SIBUILDTIME', {'en':'Service build time : ', 'ja':'サービスビルド時刻 : '});
    addClientMessage('SIPHYMEM', {'en':'Used physical memory size in KB : ', 'ja':'使用物理メモリサイズ(KB) : '});
    addClientMessage('SIVIRMEM', {'en':'Used virtual memory size in KB : ', 'ja':'使用仮想メモリサイズ(KB) : '});

    addClientMessage('FILE_UPLOAD', {'en':'Upload', 'ja':'アップロード'});
    addClientMessage('FILE_DELETE', {'en':'Delete', 'ja':'削除'});
    addClientMessage('FILE_NAME', {'en':'File name', 'ja':'ファイル名'});
    addClientMessage('FILE_SIZE', {'en':'File size', 'ja':'ファイルサイズ'});
    addClientMessage('FILE_UPDATE_TIME', {'en':'Update time', 'ja':'更新時刻'});

    addClientMessage('COMNAME', {'en':'Command Name', 'ja':'コマンド名'});
    addClientMessage('COMCOPYTOAGT', {'en':'File To Be Copied To Agent (Only file name. Do not specify directory path.)', 'ja':'エージェントにコピーされるファイル (ファイル名のみ)'});
    addClientMessage('COMTYPE', {'en':'Command Type', 'ja':'コマンド種別'});
    addClientMessage('COMSCRIPT', {'en':'Script', 'ja':'スクリプト'});
    addClientMessage('COMCOPYTOSVR', {'en':'File To Be Copied To Server (Only file name. Do not specify directory path.)', 'ja':'サーバーにコピーされるファイル (ファイル名のみ)'});
    addClientMessage('COMPLACESVR', {'en':'Name of file Placed In Server', 'ja':'サーバーに配置されたファイルの名称'});
    addClientMessage('COMPLACEAGT', {'en':'Name of file Placed In Agent', 'ja':'エージェントに配置されたファイルの名称'});
    addClientMessage('COMADD', {'en':'Add', 'ja':'追加'});
    addClientMessage('COMUPDATE', {'en':'Update', 'ja':'更新'});
    addClientMessage('COMDELETE', {'en':'Delete', 'ja':'削除'});
    addClientMessage('COMADDED', {'en':'New command has been added.', 'ja':'新規にコマンドが追加されました。'});
    addClientMessage('COMUPDATED', {'en':'The specified command has been updated.', 'ja':'指定したコマンドが更新されました。'});
    addClientMessage('COMDELETED', {'en':'The specified command has been deleted', 'ja':'指定したコマンドが削除されました。'});
    addClientMessage('COMMANDLABEL', {'en':'Command : ', 'ja':'コマンド : '});

    addClientMessage('RESULT_UPDTIME', {'en':'Execution date and time', 'ja':'実行日時'});
    addClientMessage('RESULT_AGTNAME', {'en':'Agent Name', 'ja':'エージェント名'});
    addClientMessage('RESULT_CMDNAME', {'en':'Command Name', 'ja':'コマンド名'});
    addClientMessage('RESULT_CONSOLECLOSE', {'en':'Close', 'ja':'閉じる'});
    addClientMessage('RESULT_OUTPUT', {'en':'Command output', 'ja':'コマンド出力'});

    addClientMessage('CONNERR', {
        'en':'Connection with REST API service failed. This may be caused by one of the following issues:<br>(1) REST API service cannot be started.<br>(2) REST API service is not registered as a firewall exception.<br>(3) The definition file [nginx.conf and/or sample.conf] for the host name and port number in the network connectivity settings is invalid.<br>(4) A timeout has occurred when waiting for data from REST API server.<br>',
        'ja':'REST APIサービスとの通信が失敗しました。次の要因が考えられます。<br>(1) REST APIサービスが開始されていない。<br>(2) REST APIサービスがファイアウォールに例外登録されていない。<br>(3) 接続先ホスト名およびポート番号の定義ファイル [nginx.conf , sample.conf] が不正。<br>(4) REST APIサーバからのデータ取得中にタイムアウトが発生した。<br>'
    });
}

function getArray(targetObject) {
    if (targetObject === undefined) {
        return null;
    }
    var targetArray = [];
    if (targetObject instanceof Array) {
        return targetObject;
    } else {
        targetArray.push(targetObject);
        return targetArray;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayAgentInfo() {
    let contents = [{ method: 'GET', url: '/api/agent/', request: null, keystring: 'API_GET_AGTINFO' },
                    { method: 'GET', url: '/api/command/', request: null, keystring: 'API_GET_COMMAND' }
    ];
    MultiApiCall(contents, displayAgentInfo);
}

function displayAgentInfo() {
    drowContainerFluid($('<div id="agtinfo" class="col-xs-12" style="display:block"></div>'));

    if (statusCode['API_GET_AGTINFO'] == -1 || statusCode['API_GET_AGTINFO'] == 0) {
        displayAlertDanger('#agtinfo', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_GET_AGTINFO'] != 200) {
        displayAlertDanger('#agtinfo', getSvrMsg(responseData['API_GET_AGTINFO']));
        return;
    }
    switchAgentInfoButton();

    let AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    if (AgentInfo == null) {
        $('#agtinfo').append(getClientMessage('NOAGTINFO'));
        return;
    }
    let commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);

    selectedAgentStatusCommand = -1;
    selectedOperationCommand = -1;

    let agentInfoData = $('<table>');
    agentInfoData.addClass('table stktable table-striped');

    let tmpChkBoxClm = '';
    if (userRole != 1) {
        tmpChkBoxClm = '<th></th>';
    }

    let tHead = $('<thead class="thead-light">');
    tHead.append('<tr>' +
                 tmpChkBoxClm +
                 '<th>' + getClientMessage('AINAME') + '</th>' +
                 '<th>' + getClientMessage('AISTATUS') + '</th>' +
                 '<th class="d-none d-sm-table-cell">' + getClientMessage('AISTATUSCMD') + '</th>' +
                 '<th>' + getClientMessage('AISTATUSTIME') + '</th>' +
                 '<th class="d-none d-md-table-cell">' + getClientMessage('AISTATUSUPTIME') + '</th>' +
                 '<th class="d-none d-lg-table-cell">' + getClientMessage('AIOPSTATUS') + '</th>' +
                 '<th class="d-none d-lg-table-cell">' + getClientMessage('AIOPSTATUSCMD') + '</th>' +
                 '</tr>');
    agentInfoData.append(tHead);

    let tBody = $('<tbody>');
    agentInfoData.append(tBody);
    let agentInfoDataDiv = $('<div id="agentinfotable" class="table-responsive">')
    agentInfoDataDiv.append(agentInfoData);
    $('#agtinfo').append(agentInfoDataDiv);
    for (let Loop = 0; Loop < AgentInfo.length; Loop++) {
        let cmdNameStatus = '';
        for (let loopcmd = 0; commandList != null && loopcmd < commandList.length; loopcmd++) {
            if (AgentInfo[Loop].StatusCmd == commandList[loopcmd].Id) {
                cmdNameStatus = commandList[loopcmd].Name;
                break;
            }
        }
        let cmdNameOp = '';
        for (let loopcmd = 0; commandList != null && loopcmd < commandList.length; loopcmd++) {
            if (AgentInfo[Loop].OpCmd == commandList[loopcmd].Id) {
                cmdNameOp = commandList[loopcmd].Name;
                break;
            }
        }
        let acqTimeInt = parseInt(AgentInfo[Loop].AcqTime, 16);
        let updTimeInt = parseInt(AgentInfo[Loop].UpdTime, 16);
        let dateAcqTime = new Date(acqTimeInt * 1000);
        let dateUpdTime = new Date(updTimeInt * 1000);
        let acqTimeStr = dateAcqTime.toString()
        let updTimeStr = dateUpdTime.toString()

        let tmpChkBoxStr = '';
        if (userRole != 1) {
            tmpChkBoxStr = '<td><div class="checkbox"><input type="checkbox" id="agtInfoId' + Loop + '" value="" onclick="switchAgentInfoButton()"/></div></td>';
        }
        tBody.append('<tr>' +
                     tmpChkBoxStr +
                     '<td><a id="agentprop' + Loop + '" style="cursor: pointer;">' + AgentInfo[Loop].Name + '</a></td>' +
                     '<td><div align="center" id="statusTd' + Loop + '" data-toggle="tooltip" title="' + getTooltipStr() + '">' + AgentInfo[Loop].Status + '</div></td>' +
                     '<td class="d-none d-sm-table-cell">' + cmdNameStatus + '</td>' +
                     '<td>' + acqTimeStr + '</td>' +
                     '<td class="d-none d-md-table-cell">' + updTimeStr + '</td>' +
                     '<td class="d-none d-lg-table-cell"><div align="center" id="opStatusTd' + Loop + '" data-toggle="tooltip" title="' + getTooltipStr() + '">' + AgentInfo[Loop].OpStatus + '</div></td>' +
                     '<td class="d-none d-lg-table-cell">' + cmdNameOp + '</td>' +
                     '</tr>');
        // Draw rectangle for agent status
        if (AgentInfo[Loop].Status == 0) {
            $('#statusTd' + Loop).css('background-color', 'LightGreen');
        } else if (AgentInfo[Loop].Status <= -980 && AgentInfo[Loop].Status >= -989) {
            $('#statusTd' + Loop).css('background-color', 'LightGreen');
        } else {
            $('#statusTd' + Loop).css('background-color', 'LightCoral');
        }
        // Draw rectangle for operation status
        if (AgentInfo[Loop].OpStatus == 0) {
            $('#opStatusTd' + Loop).css('background-color', 'LightGreen');
        } else if (AgentInfo[Loop].OpStatus <= -980 && AgentInfo[Loop].OpStatus >= -989) {
            $('#opStatusTd' + Loop).css('background-color', 'LightGreen');
        } else {
            $('#opStatusTd' + Loop).css('background-color', 'LightCoral');
        }
    }
    for (let loop = 0; loop < AgentInfo.length; loop++) {
        $('#agentprop' + loop).on('click', function() {
            showAgentPropertiesDialog(AgentInfo[loop].Name);
        });
    }
    let agentInfoButtonDiv = $('<div id="agentinfobutton" style="padding: 6px 6px 10px 10px; background-color: #e9ecef">')
    agentInfoButtonDiv.append('<button type="button" id="setAgentStatusCommand" class="btn btn-dark disabled" onclick="displayAgentStatusCommandDlg()">' + getClientMessage('AISETSTATUSCMD') + '</button> ');
    agentInfoButtonDiv.append('<button type="button" id="execOpeCommand" class="btn btn-dark disabled" onclick="displayExecCommandDlg()">' + getClientMessage('AIEXECCMD') + '</button> ');
    $('#agtinfo').append(agentInfoButtonDiv);
    resizeComponent();
}

function getTooltipStr() {
    var tooltipStr = getClientMessage('RESCODE-980') +
                     getClientMessage('RESCODE-981') +
                     getClientMessage('RESCODE-982') +
                     getClientMessage('RESCODE-983') +
                     getClientMessage('RESCODE-984') +
                     getClientMessage('RESCODE-985') +
                     getClientMessage('RESCODE-990') +
                     getClientMessage('RESCODE-991') +
                     getClientMessage('RESCODE-992') +
                     getClientMessage('RESCODE-993') +
                     getClientMessage('RESCODE-994') +
                     getClientMessage('RESCODE-995');
    return tooltipStr;
}

function switchAgentInfoButton() {
    var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    var foundFlag = false;
    for (var loop = 0; AgentInfo != null && loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            foundFlag = true;
        }
    }
    if (foundFlag == true) {
        $('#setAgentStatusCommand').removeClass('disabled');
        $('#execOpeCommand').removeClass('disabled');
    } else {
        $('#setAgentStatusCommand').addClass('disabled');
        $('#execOpeCommand').addClass('disabled');
    }
}

function displayExecCommandDlg() {
    var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    var foundFlag = false;
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            foundFlag = true;
        }
    }
    if (foundFlag == false) {
        return;
    }

    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);

    var execCommandDlg = $('<div/>')

    var btnGrp = $('<div class="btn-group">');
    btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selectedExecCommand">' + getClientMessage('SELCMD') + '</span><span class="caret"></span></button>');
    var ddMenu = $('<ul class="dropdown-menu" role="menu">');
    for (var loop = 0; commandList != null && loop < commandList.length; loop++) {
        ddMenu.append('<li role="presentation"><a onclick="selectExecCommand(' + loop + ')" role="menuitem" tabindex="-1" href="#">' + commandList[loop].Name + '</a></li>');
    }
    btnGrp.append(ddMenu);
    execCommandDlg.append('<p>');
    execCommandDlg.append(getClientMessage('COMMANDLABEL'));
    execCommandDlg.append(btnGrp);
    execCommandDlg.append('</p>');

    execCommandDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeExecCommandDlg(true)">Execute</button> ');
    execCommandDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeExecCommandDlg(false)">Cancel</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('SELOPCMD') + '</h5>', execCommandDlg);
}

function closeExecCommandDlg(okFlag) {
    if (okFlag == true && selectedOperationCommand != -1) {
        var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        for (var loop = 0; loop < AgentInfo.length; loop++) {
            if ($('#agtInfoId' + loop).prop('checked') == true) {
                var ReqObj = { Name : AgentInfo[loop].Name, OpCmd : commandList[selectedOperationCommand].Id };
                apiCall('POST', '/api/agent/', ReqObj, 'API_POST_AGTINFO', null);
            }
        }
        apiCall(null, null, null, '', transDisplayAgentInfo);
    }
    closeInputModal();
}

function selectExecCommand(execCommand) {
    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    $('#selectedExecCommand').text(commandList[execCommand].Name);
    selectedOperationCommand = execCommand;
}

function displayAgentStatusCommandDlg() {
    var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    var foundFlag = false;
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            foundFlag = true;
        }
    }
    if (foundFlag == false) {
        return;
    }

    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);

    var execSaCommandDlg = $('<div/>')

    var btnGrp = $('<div class="btn-group">');
    btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selectedAgentStatus">' + getAgentStatusCommand() + '</span><span class="caret"></span></button>');
    var ddMenu = $('<ul class="dropdown-menu" role="menu">');
    for (var loop = 0; commandList != null && loop < commandList.length; loop++) {
        ddMenu.append('<li role="presentation"><a onclick="selectAgentStatusCommand(' + loop + ')" role="menuitem" tabindex="-1" href="#">' + commandList[loop].Name + '</a></li>');
    }
    btnGrp.append(ddMenu);
    execSaCommandDlg.append('<p>');
    execSaCommandDlg.append(getClientMessage('COMMANDLABEL'));
    execSaCommandDlg.append(btnGrp);
    execSaCommandDlg.append('</p>');

    execSaCommandDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeAgentStatusCommandDlg(true)">OK</button> ');
    execSaCommandDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeAgentStatusCommandDlg(false)">Cancel</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('SELSTATUSCMD') + '</h5>', execSaCommandDlg);
}

function closeAgentStatusCommandDlg(okFlag) {
    if (okFlag == true && selectedAgentStatusCommand != -1) {
        var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        for (var loop = 0; loop < AgentInfo.length; loop++) {
            if ($('#agtInfoId' + loop).prop('checked') == true) {
                var ReqObj = { Name : AgentInfo[loop].Name, StatusCmd : commandList[selectedAgentStatusCommand].Id };
                apiCall('POST', '/api/agent/', ReqObj, 'API_POST_AGTINFO', null);
            }
        }
        apiCall(null, null, null, '', transDisplayAgentInfo);
    }
    closeInputModal();
}

function getAgentStatusCommand() {
    var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    var allSame = true;
    var commandId = -1;
    var commandStr = '';
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            if (commandId != -1 && AgentInfo[loop].StatusCmd != commandId) {
                allSame = false;
            } else {
                commandId = AgentInfo[loop].StatusCmd;
            }
        }
    }
    if (commandId == -1 || allSame == false) {
        return '';
    } else {
        if (commandList != null) {
            for (var loop = 0; loop < commandList.length; loop++) {
                if (commandId == commandList[loop].Id) {
                    return commandList[loop].Name;
                }
            }
        }
    }
    return '';
}

function selectAgentStatusCommand(agentStatusCommand) {
    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    $('#selectedAgentStatus').text(commandList[agentStatusCommand].Name);
    selectedAgentStatusCommand = agentStatusCommand;
}

function showAgentPropertiesDialog(targetName) {
    let agentPropDlg = $('<div/>');
    if (responseData['API_GET_AGTINFO'].Data === undefined) {
        agentPropDlg.append(getClientMessage('NOAGTINFO'));
        showInputModal('<h5 class="modal-title">' + targetName + '</h5>', agentPropDlg);
        return;
    }
    let agentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    if (agentInfo == null) {
        agentPropDlg.append(getClientMessage('NOAGTINFO'));
        showInputModal('<h5 class="modal-title">' + targetName + '</h5>', agentPropDlg);
        return;
    }
    for (let loop = 0; loop < agentInfo.length; loop++) {
        if (targetName === agentInfo[loop].Name) {
            agentPropDlg.append(getClientMessage('AINAME') + ' : ' + targetName + '<br/>');
            let acqTimeInt = parseInt(agentInfo[loop].AcqTime, 16);
            let updTimeInt = parseInt(agentInfo[loop].UpdTime, 16);
            let iniTimeInt = parseInt(agentInfo[loop].IniTime, 16);
            let reqTimeInt = parseInt(agentInfo[loop].ReqTime, 16);
            let dateAcqTime = new Date(acqTimeInt * 1000);
            let dateUpdTime = new Date(updTimeInt * 1000);
            let dateIniTime = new Date(iniTimeInt * 1000);
            let dateReqTime = new Date(reqTimeInt * 1000);
            let acqTimeStr = dateAcqTime.toString();
            let updTimeStr = dateUpdTime.toString();
            let iniTimeStr = dateIniTime.toString();
            let reqTimeStr = dateReqTime.toString();
            agentPropDlg.append(getClientMessage('AISTATUSINITIME') + ' : ' + iniTimeStr + '<br/>');
            agentPropDlg.append(getClientMessage('AILASTPOLLINGTIME') + ' : ' + reqTimeStr + '<br/>');
            agentPropDlg.append(getClientMessage('AISTATUSTIME') + ' : ' + acqTimeStr + '<br/>');
            agentPropDlg.append(getClientMessage('AISTATUSUPTIME') + ' : ' + acqTimeStr + '<br/>');
            agentPropDlg.append('<p></p>');
            agentPropDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeInputModal()">OK</button> ');
        }
    }

    showInputModal('<h5 class="modal-title">' + targetName + '</h5>', agentPropDlg);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayServerInfo() {
    apiCall('GET', '/api/server/', null, 'API_GET_SVRINFO', displayServerInfo);
}

function displayServerInfo() {
    drowContainer($('<div id="svrinfo" class="row col-xs-12" style="display:block"></div>'));

    $('#svrinfo').append('<h2>' + getClientMessage('SERVERINFO') + '</h2>');
    if (statusCode['API_GET_SVRINFO'] == -1 || statusCode['API_GET_SVRINFO'] == 0) {
        displayAlertDanger('#svrinfo', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_GET_SVRINFO'] != 200) {
        displayAlertDanger('#svrinfo', getSvrMsg(responseData['API_GET_SVRINFO']));
        return;
    }

    if (svrinfo_msg !== '') {
        displayAlertSuccess('#svrinfo', svrinfo_msg);
        svrinfo_msg = '';
    }

    $('#svrinfo').append('<p>' + getClientMessage('SISVRVERSION') + responseData['API_GET_SVRINFO'].Data.ServerInfo.Version + '</p>');

    let buildTimeStr = responseData['API_GET_SVRINFO'].Data.ServerInfo.BuildTime;
    $('#svrinfo').append('<p>' + getClientMessage('SIBUILDTIME') + buildTimeStr + '</p>');

    let startTimeInt = parseInt(responseData['API_GET_SVRINFO'].Data.ServerInfo.StartTime, 16);
    let dateStartTime = new Date(startTimeInt * 1000);
    let StartTimeStr = dateStartTime.toString()
    $('#svrinfo').append('<p>' + getClientMessage('SISTARTTIME') + StartTimeStr + '</p>');

    let PhyMem = responseData['API_GET_SVRINFO'].Data.ServerInfo.UsedPhysicalMemory;
    let VirMem = responseData['API_GET_SVRINFO'].Data.ServerInfo.UsedVirtualMemory;
    $('#svrinfo').append('<p>' + getClientMessage('SIPHYMEM') + PhyMem + '</p>');
    $('#svrinfo').append('<p>' + getClientMessage('SIVIRMEM') + VirMem + '</p>');


    // Polling Interval
    if (responseData['API_GET_SVRINFO'].Data.ServerInfo.PollingInterval == 30) {
        selectedPollingInterval = 0;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.PollingInterval == 60) {
        selectedPollingInterval = 1;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.PollingInterval == 300) {
        selectedPollingInterval = 2;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.PollingInterval == 900) {
        selectedPollingInterval = 3;
    }
    var btnGrp = $('<div class="btn-group">');
    btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selectedPI">' + pollingIntervalStr[selectedPollingInterval] + '</span><span class="caret"></span></button>');
    var ddMenu = $('<ul class="dropdown-menu" role="menu">');
    ddMenu.append('<li role="presentation"><a onclick="selectPollingInterval(0)" role="menuitem" tabindex="-1" href="#">' + pollingIntervalStr[0] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectPollingInterval(1)" role="menuitem" tabindex="-1" href="#">' + pollingIntervalStr[1] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectPollingInterval(2)" role="menuitem" tabindex="-1" href="#">' + pollingIntervalStr[2] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectPollingInterval(3)" role="menuitem" tabindex="-1" href="#">' + pollingIntervalStr[3] + '</a></li>');
    btnGrp.append(ddMenu);
    $('#svrinfo').append('<p>');
    $('#svrinfo').append(getClientMessage('SIPOLLINTVL'));
    $('#svrinfo').append(btnGrp);
    $('#svrinfo').append('</p>');

    // Status Acquisition Interval
    if (responseData['API_GET_SVRINFO'].Data.ServerInfo.StatusAcquisitionInterval == 300) {
        selectedStatusAcquisitionInterval = 0;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.StatusAcquisitionInterval == 900) {
        selectedStatusAcquisitionInterval = 1;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.StatusAcquisitionInterval == 1800) {
        selectedStatusAcquisitionInterval = 2;
    } else if (responseData['API_GET_SVRINFO'].Data.ServerInfo.StatusAcquisitionInterval == 3600) {
        selectedStatusAcquisitionInterval = 3;
    }
    var btnGrp = $('<div class="btn-group">');
    btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selectedSAI">' + statusAcquisitionIntervalStr[selectedStatusAcquisitionInterval] + '</span><span class="caret"></span></button>');
    var ddMenu = $('<ul class="dropdown-menu" role="menu">');
    ddMenu.append('<li role="presentation"><a onclick="selectStatusAcquisitionInterval(0)" role="menuitem" tabindex="-1" href="#">' + statusAcquisitionIntervalStr[0] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectStatusAcquisitionInterval(1)" role="menuitem" tabindex="-1" href="#">' + statusAcquisitionIntervalStr[1] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectStatusAcquisitionInterval(2)" role="menuitem" tabindex="-1" href="#">' + statusAcquisitionIntervalStr[2] + '</a></li>');
    ddMenu.append('<li role="presentation"><a onclick="selectStatusAcquisitionInterval(3)" role="menuitem" tabindex="-1" href="#">' + statusAcquisitionIntervalStr[3] + '</a></li>');
    btnGrp.append(ddMenu);

    $('#svrinfo').append('<p>');
    $('#svrinfo').append(getClientMessage('SISTACQINTVL'));
    $('#svrinfo').append(btnGrp);
    $('#svrinfo').append('</p>');

    $('#svrinfo').append('<div id="svrinfo_errmsg">');

    $('#svrinfo').append('<p>');
    $('#svrinfo').append('<button type="button" id="serverInfoBtnUpdate" class="btn btn-primary" onclick="updateServerInfo()">' + getClientMessage('SIUPDATEBTN') + '</button> ');
    $('#svrinfo').append('</p>');
    $('td').css('vertical-align', 'middle');
}

function selectPollingInterval(interval) {
    selectedPollingInterval = interval;
    $('#selectedPI').text(pollingIntervalStr[selectedPollingInterval]);
}

function selectStatusAcquisitionInterval(interval) {
    selectedStatusAcquisitionInterval = interval;
    $('#selectedSAI').text(statusAcquisitionIntervalStr[selectedStatusAcquisitionInterval]);
}

function getPollingInterval(interval) {
    if (interval == 0) {
        return 30;
    } else if (interval == 1) {
        return 60;
    } else if (interval == 2) {
        return 300;
    } else if (interval == 3) {
        return 900;
    }
}

function getStatusAcquisitionInterval(interval) {
    if (interval == 0) {
        return 300;
    } else if (interval == 1) {
        return 900;
    } else if (interval == 2) {
        return 1800;
    } else if (interval == 3) {
        return 3600;
    }
}

function updateServerInfo() {
    var pInterval = getPollingInterval(selectedPollingInterval);
    var saInterval = getStatusAcquisitionInterval(selectedStatusAcquisitionInterval);
    apiCall('POST', '/api/server/', {"PollingInterval":pInterval, "StatusAcquisitionInterval":saInterval}, 'API_POST_SVRINFO', refreshAfterUpdateServerInfo);
}

function refreshAfterUpdateServerInfo() {
    $('#svrinfo .alert').remove();
    if (statusCode['API_POST_SVRINFO'] == -1 || statusCode['API_POST_SVRINFO'] == 0) {
        displayAlertDanger('#svrinfo_errmsg', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_POST_SVRINFO'] != 200) {
        displayAlertDanger('#svrinfo_errmsg', getSvrMsg(responseData['API_POST_SVRINFO']));
        return;
    }
    transDisplayServerInfo();
    svrinfo_msg = getClientMessage('SIUPDATED');
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayFileMgmt() {
    apiCall('GET', '/api/filelist/', null, 'API_GET_FILELIST', displayFileMgmt);
}

function displayFileMgmt() {
    drowContainerFluid($('<div id="filemgmt" class="col-xs-12" style="display:block"></div>'));

    if (statusCode['API_GET_FILELIST'] == -1 || statusCode['API_GET_FILELIST'] == 0) {
        displayAlertDanger('#filemgmt', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_GET_FILELIST'] != 200) {
        displayAlertDanger('#filemgmt', getSvrMsg(responseData['API_GET_FILELIST']));
        return;
    }

    let fileMgmtDataDiv = $('<div id="filemgmttable" class="table-responsive">');
    if (responseData['API_GET_FILELIST'].Data === undefined) {
        fileMgmtDataDiv.append(getClientMessage('NOFILEEXIST'));
        $('#filemgmt').append(fileMgmtDataDiv);
    } else {
        let fileList = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);

        let tableListData = $('<table>');
        tableListData.addClass('table stktable table-striped');
    
        let tmpChkBoxClm = '';
        if (userRole != 1) {
            tmpChkBoxClm = '<th></th>';
        }

        let tHead = $('<thead class="thead-light">');
        tHead.append('<tr>' +
                     tmpChkBoxClm +
                     '<th>' + getClientMessage('FILE_NAME') + '</th>' + '<th>' + getClientMessage('FILE_SIZE') + '</th>' + '<th>' + getClientMessage('FILE_UPDATE_TIME') + '</th>' +
                     '</tr>');
        tableListData.append(tHead);

        let tBody = $('<tbody>');
        for (let Loop = 0; Loop < fileList.length; Loop++) {
            let tmpChkBoxStr = '';
            if (userRole != 1) {
                tmpChkBoxStr = '<td><div class="checkbox"><input type="checkbox" id="fileInfoId' + Loop + '" value="" onclick="switchFileInfoButton()"/></div></td>';
            }

            let updTimeInt = parseInt(fileList[Loop].UpdTime, 16);
            let dateUpdTime = new Date(updTimeInt * 1000);
            tBody.append('<tr>' + tmpChkBoxStr + '<td><a id="fileInfoAncId' + Loop + '" style="cursor: pointer;">' + fileList[Loop].Name + '</a></td><td>' + fileList[Loop].Size + '</td><td>' + dateUpdTime + '</td></tr>');
        }

        tableListData.append(tBody);
        fileMgmtDataDiv.append(tableListData);
        $('#filemgmt').append(fileMgmtDataDiv);

        for (let loop = 0; loop < fileList.length; loop++) {
            $('#fileInfoAncId' + loop).on('click', function() {
                fileDownload(fileList[loop].Name, fileList[loop].Size, 0);
            });
        }
    }

    let fileMgmtButtonDiv = $('<div id="filemgmtbutton" style="padding: 6px 6px 10px 10px; background-color: #e9ecef;">')
    fileMgmtButtonDiv.append('<form id="fileuploadform" method="post" action="/api/filea/" enctype="multipart/form-data"><input type="file" id="fileupload" style="display:none" multiple/></form>');
    let tmpJq = $('<button id="fileInfoUpload" class="btn btn-dark" type="button">' + getClientMessage('FILE_UPLOAD') + '</button>');
    if (userRole != 1) {
        tmpJq.click(function() {
            $("#fileupload").trigger("click");
        });
    }
    fileMgmtButtonDiv.append(tmpJq);
    fileMgmtButtonDiv.append('&nbsp;');
    fileMgmtButtonDiv.append('<button id="fileInfoDelete" class="btn btn-dark" type="button" onclick="deleteFile()">' + getClientMessage('FILE_DELETE') + '</button>');
    $('#filemgmt').append(fileMgmtButtonDiv);
    resizeComponent();
    switchFileInfoButton();

    let fileUpload = document.getElementById('fileupload');
    fileUpload.addEventListener('change', function () {
        InitializeNumOfApiCall();
        for (let loop = 0; loop < this.files.length; loop++) {
            if (this.files[loop] === undefined) {
                return;
            }
            let fr = new FileReader();
            fr.readAsArrayBuffer(this.files[loop]);
            fr.onload = (function(targetFile) {
                return function(evt) {
                    let orgData = new Uint8Array(evt.target.result);
                    let encData = "";
                    let contents = [];
                    if (orgData.byteLength == 0) {
                        let sendData = {
                            FileName : targetFile.name,
                            FileOffset : 0,
                            FileSize : 0,
                            FileData : ""
                        };
                        contents.push({ method: 'POST', url: '/api/file/', request: sendData, keystring: 'API_POST_FILE' })
                    }
                    for (let loop = 0; loop < orgData.byteLength; loop++) {
                        // Do NOT use padStart for IE11
                        if (orgData[loop] == 0) {
                            encData = encData + '00';
                        } else if (orgData[loop] < 16) {
                            encData = encData + '0';
                            encData = encData + orgData[loop].toString(16).toUpperCase();
                        } else {
                            encData = encData + orgData[loop].toString(16).toUpperCase();
                        }
                        if (loop % 1000000 == 999999 || loop == orgData.byteLength - 1) {
                            let offset = 0;
                            if (loop < 1000000) {
                                offset = 0;
                            } else {
                                offset = parseInt(loop / 1000000) * 1000000;
                            }
                            let sendData = {
                                FileName : targetFile.name,
                                FileOffset : offset,
                                FileSize : orgData.byteLength,
                                FileData : encData
                            };
                            contents.push({ method: 'POST', url: '/api/file/', request: sendData, keystring: 'API_POST_FILE' })
                            encData = "";
                        }
                    }
                    IncreaseNumOfFileMgmtApiCall();
                    sequentialApiCall(contents, FinalizationOfFileMgmtApiCall);
                }
            })(this.files[loop]);
        }
    });
}

{
    let NumOfApiCall = 0;

    function InitializeNumOfApiCall() {
        initSequentialApiCall();
        NumOfApiCall = 0;
    }

    function IncreaseNumOfFileMgmtApiCall() {
        NumOfApiCall++;
    }

    function FinalizationOfFileMgmtApiCall() {
        NumOfApiCall--;
        if (NumOfApiCall == 0) {
            finalSequentialApiCall();
            transDisplayFileMgmt();
        }
    }
}

function fileDownload(fileName, filesize, offset) {
    let chunks = parseInt(filesize / 1000000 + 1);
    let contents = [];
    for (let loop = 0; loop < chunks; loop++) {
        let encFileName = encodeURI(fileName);
        let tmpUrl = '/api/file/' + encFileName + '/' + loop * 1000000 + '/';
        contents.push({ method: 'GET', url: tmpUrl, request: null, keystring: 'API_GET_FILE_' + loop });
    }
    initSequentialApiCall();
    sequentialApiCall(contents, fileDownloadImpl);
}

function fileDownloadObjClear(chunks) {
    for (let loop = 0; loop < chunks; loop++) {
        delete responseData['API_GET_FILE_' + loop];
        delete statusCode['API_GET_FILE_' + loop];
    }
}

function fileDownloadImpl() {
    if (statusCode['API_GET_FILE_0'] == -1 || statusCode['API_GET_FILE_0'] == 0) {
        $('#filemgmttable').empty();
        displayAlertDanger('#filemgmttable', getClientMessage('CONNERR'));
        delete responseData['API_GET_FILE_0'];
        delete statusCode['API_GET_FILE_0'];
        finalSequentialApiCall();
        return;
    }
    if (statusCode['API_GET_FILE_0'] != 200) {
        $('#filemgmttable').empty();
        displayAlertDanger('#filemgmttable', getSvrMsg(responseData['API_GET_FILE_0']));
        delete responseData['API_GET_FILE_0'];
        delete statusCode['API_GET_FILE_0'];
        finalSequentialApiCall();
        return;
    }

    let fileName = responseData['API_GET_FILE_0'].Data.FileName;
    let fileSize = responseData['API_GET_FILE_0'].Data.FileSize;
    let chunks = parseInt(fileSize / 1000000 + 1);
    let typedArrays = [chunks];
    if (fileSize == 0) {
        typedArrays[0] = new Uint8Array(0);
    } else {
        for (let loop = 0; loop < chunks; loop++) {
            if (statusCode['API_GET_FILE_' + loop] === undefined || responseData['API_GET_FILE_' + loop] === undefined) {
                continue;
            }
            if (statusCode['API_GET_FILE_' + loop] == -1 || statusCode['API_GET_FILE_' + loop] == 0) {
                $('#filemgmttable').empty();
                displayAlertDanger('#filemgmttable', getClientMessage('CONNERR'));
                fileDownloadObjClear(chunks);
                finalSequentialApiCall();
                return;
            }
            if (statusCode['API_GET_FILE_' + loop] != 200) {
                $('#filemgmttable').empty();
                displayAlertDanger('#filemgmttable', getSvrMsg(responseData['API_GET_FILE_' + loop]));
                fileDownloadObjClear(chunks);
                finalSequentialApiCall();
                return;
            }
            typedArrays[loop] = new Uint8Array(responseData['API_GET_FILE_' + loop].Data.FileData.match(/[\da-f]{2}/gi).map(function (h) {
                return parseInt(h, 16)
            }))
        }
    }
    let blob = new Blob(typedArrays, { "type" : "application/octet-stream" });
    if (window.navigator.msSaveBlob) {
        // For IE11
        window.navigator.msSaveBlob(blob, fileName);
    } else {
        let newLink = document.createElement('a');
        newLink.href = window.URL.createObjectURL(blob);
        newLink.download = fileName;
        document.body.appendChild(newLink);
        newLink.click();
        document.body.removeChild(newLink);
    }
    // Clear unnecessary data
    fileDownloadObjClear(chunks);

    finalSequentialApiCall();
}

function switchFileInfoButton() {
    var foundFlag = false;
    if (responseData['API_GET_FILELIST'].Data !== undefined && responseData['API_GET_FILELIST'].Data.FileInfo !== undefined) {
        var fileList = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);
        for (var loop = 0; fileList != null && loop < fileList.length; loop++) {
            if ($('#fileInfoId' + loop).prop('checked') == true) {
                foundFlag = true;
            }
        }
    }
    if (foundFlag == true) {
        $('#fileInfoDelete').removeClass('disabled');
    } else {
        $('#fileInfoDelete').addClass('disabled');
    }
    if  (userRole != 1) {
        $('#fileInfoUpload').removeClass('disabled');
    } else {
        $('#fileInfoUpload').addClass('disabled');
    }
}

function deleteFile() {
    if (userRole == 1) {
        return;
    }
    let fileList = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);
    let contents = [];
    for (var loop = 0; loop < fileList.length; loop++) {
        if ($('#fileInfoId' + loop).prop('checked') == true) {
            let encFileName = encodeURI(fileList[loop].Name);
            let tmpUrl = '/api/file/' + encFileName + '/';
            contents.push({ method: 'DELETE', url: tmpUrl, request: null, keystring: 'API_DELETE_FILE' + loop });
        }
    }
    initSequentialApiCall();
    sequentialApiCall(contents, checkAfterDeleteFile);
}

function checkAfterDeleteFile() {
    finalSequentialApiCall();
    let errorFlag = false;
    let fileList = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);
    for (var loop = 0; loop < fileList.length; loop++) {
        if (statusCode['API_DELETE_FILE' + loop] === undefined || responseData['API_DELETE_FILE' + loop] === undefined) {
            continue;
        }
        if (statusCode['API_DELETE_FILE' + loop] == -1 || statusCode['API_DELETE_FILE' + loop] == 0) {
            $('#filemgmttable').empty();
            displayAlertDanger('#filemgmttable', getClientMessage('CONNERR'));
            errorFlag = true;
            break;
        }
        if (statusCode['API_DELETE_FILE' + loop] != 200) {
            $('#filemgmttable').empty();
            displayAlertDanger('#filemgmttable', getSvrMsg(responseData['API_DELETE_FILE' + loop]));
            errorFlag = true;
            break;
        }
    }
    for (var loop = 0; loop < fileList.length; loop++) {
        delete responseData['API_DELETE_FILE' + loop];
        delete statusCode['API_DELETE_FILE' + loop];
    }
    if (errorFlag == false) {
        transDisplayFileMgmt();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayCommand() {
    apiCall('GET', '/api/command/', null, 'API_GET_COMMAND', displayCommand);
}

function displayCommand() {
    drowContainer($('<div id="command" class="row col-xs-12" style="display:block"></div>'));

    $('#command').append('<h2>' + getClientMessage('COMMAND') + '</h2>');
    if (statusCode['API_GET_COMMAND'] == -1 || statusCode['API_GET_COMMAND'] == 0) {
        displayAlertDanger('#command', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_GET_COMMAND'] != 200) {
        displayAlertDanger('#command', getSvrMsg(responseData['API_GET_COMMAND']));
        return;
    }
    if (command_msg !== '') {
        displayAlertSuccess('#command', command_msg);
        command_msg = '';
    }
    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    if (commandList == null) {
        $('#command').append(getClientMessage('NOCMDEXIST'));
    }

    selectedCommand = -1;

    if (responseData['API_GET_COMMAND'].Data.Command !== undefined) {
        var commandListTable = $('<table>');
        commandListTable.addClass('table table-striped');

        var tHead = $('<thead>');
        tHead.append('<tr><th>' + getClientMessage('COMNAME') + '</th><th>' + getClientMessage('COMTYPE') + '</th></tr>');
        commandListTable.append(tHead);

        var tBody = $('<tbody>');
        for (var Loop = 0; Loop < commandList.length; Loop++) {
            var typeStr = '';
            if (commandList[Loop].Type == 0) {
                typeStr = 'Linux /usr/bin/bash';
            } else {
                typeStr = 'Windows cmd.exe /c';
            }
            tBody.append('<tr><td><div class="radio"><label><input type="radio" id="radioCom' + commandList[Loop].Id + '" name="optradio" onclick="selectCommand('+ commandList[Loop].Id + ')"/>' + commandList[Loop].Name + '</label></div></td><td>' + typeStr + '</td></tr>');
        }
        commandListTable.append(tBody);
        $('#command').append(commandListTable);
    }

    $('#command').append('<div class="form-group"><label for="commandName">' + getClientMessage('COMNAME') + '</label><input type="text" class="form-control" id="commandName" placeholder="' + getClientMessage('COMNAME') + '"></div>');
    $('#command').append('<div class="form-group"><label for="serverFileName">' + getClientMessage('COMCOPYTOAGT') + '</label><input type="text" class="form-control" id="serverFileName" placeholder="' + getClientMessage('COMPLACESVR') + '"></div>');
    $('#command').append('<div class="form-group"><label for="commandType">' + getClientMessage('COMTYPE') + '</label><select class="form-control" id="commandType"><option>Linux /usr/bin/bash</option><option>Windows cmd.exe /c</option></select></div>');
    $('#command').append('<div class="form-group"><label for="commandScript">' + getClientMessage('COMSCRIPT') + '</label><textarea class="form-control" id="commandScript" rows="3" style="margin-top: 0px; margin-bottom: 0px; height: 185px;"></textarea></div>');
    $('#command').append('<div class="form-group"><label for="agentFileName">' + getClientMessage('COMCOPYTOSVR') + '</label><input type="text" class="form-control" id="agentFileName" placeholder="' + getClientMessage('COMPLACEAGT') + '"></div>');
    $('#command').append('<div id="command_errmsg"/>');
    $('#command').append('<button type="button" id="commandBtnAdd" class="btn btn-primary" onclick="updateCommand(false)">' + getClientMessage('COMADD') + '</button> ');
    $('#command').append('<button type="button" id="commandBtnUpdate" class="btn btn-primary disabled" onclick="updateCommand(true)">' + getClientMessage('COMUPDATE') + '</button> ');
    $('#command').append('<button type="button" id="commandBtnDelete" class="btn btn-primary disabled" onclick="deleteCommand()">' + getClientMessage('COMDELETE') + '</button> ');
    $('#command').append('<p></p>');
}

function updateCommand(updateFlag) {
    if (updateFlag == true && selectedCommand == -1) {
        return;
    }
    var comType = $("#commandType").val();
    var comTypeInt = -1;
    if (comType === 'Linux /usr/bin/bash') {
        comTypeInt = 0;
    } else if (comType === 'Windows cmd.exe /c') {
        comTypeInt = 1;
    }
    if (updateFlag == false) {
        var ReqObj = { Name : $("#commandName").val(), Type : comTypeInt, Script : $("#commandScript").val(), ServerFileName : $("#serverFileName").val(), AgentFileName : $("#agentFileName").val() };
        apiCall('POST', '/api/command/', ReqObj, 'API_POST_COMMAND', refreshAfterAddCommand);
    } else {
        var ReqObj = { Id : selectedCommand, Name : $("#commandName").val(), Type : comTypeInt, Script : $("#commandScript").val(), ServerFileName : $("#serverFileName").val(), AgentFileName : $("#agentFileName").val() };
        apiCall('POST', '/api/command/', ReqObj, 'API_POST_COMMAND', refreshAfterUpdateCommand);
    }
}

function deleteCommand() {
    if (selectedCommand == -1) {
        return;
    }
    apiCall('DELETE', '/api/command/' + selectedCommand + '/', null, 'API_DELETE_COMMAND', refreshAfterDeleteCommand);
}

function selectCommand(targetId) {
    var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    selectedCommand = targetId;
    for (loop = 0; loop < commandList.length; loop++) {
        if (commandList[loop].Id == targetId) {
            var typeStr = '';
            if (commandList[loop].Type == 0) {
                typeStr = 'Linux /usr/bin/bash';
            } else {
                typeStr = 'Windows cmd.exe /c';
            }
            $('#commandName').val(commandList[loop].Name);
            $('#serverFileName').val(commandList[loop].ServerFileName);
            $('#commandType').val(typeStr);
            $('#commandScript').val(commandList[loop].Script);
            $('#agentFileName').val(commandList[loop].AgentFileName);
            $('#commandBtnUpdate').removeClass('disabled');
            $('#commandBtnDelete').removeClass('disabled');
        }
    }
}

function refreshAfterAddCommand() {
    $('#command .alert').remove();
    if (statusCode['API_POST_COMMAND'] == -1 || statusCode['API_POST_COMMAND'] == 0) {
        displayAlertDanger('#command_errmsg', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_POST_COMMAND'] != 200) {
        displayAlertDanger('#command_errmsg', getSvrMsg(responseData['API_POST_COMMAND']));
        return;
    }
    transDisplayCommand();
    command_msg = getClientMessage('COMADDED');
}

function refreshAfterUpdateCommand() {
    $('#command .alert').remove();
    if (statusCode['API_POST_COMMAND'] == -1 || statusCode['API_POST_COMMAND'] == 0) {
        displayAlertDanger('#command_errmsg', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_POST_COMMAND'] != 200) {
        displayAlertDanger('#command_errmsg', getSvrMsg(responseData['API_POST_COMMAND']));
        return;
    }
    transDisplayCommand();
    command_msg = getClientMessage('COMUPDATED');
}

function refreshAfterDeleteCommand() {
    $('#command .alert').remove();
    if (statusCode['API_DELETE_COMMAND'] == -1 || statusCode['API_DELETE_COMMAND'] == 0) {
        displayAlertDanger('#command_errmsg', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_DELETE_COMMAND'] != 200) {
        displayAlertDanger('#command_errmsg', getSvrMsg(responseData['API_DELETE_COMMAND']));
        return;
    }
    transDisplayCommand();
    command_msg = getClientMessage('COMDELETED');
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayCommandResult() {
    apiCall('GET', '/api/commandresult/', null, 'API_GET_COMMANDRESULT', displayCommandResult);
}

function displayCommandResult() {
    drowContainerFluid($('<div id="commandresult" class="col-xs-12" style="display:block"></div>'));
    if (statusCode['API_GET_COMMANDRESULT'] == -1 || statusCode['API_GET_COMMANDRESULT'] == 0) {
        displayAlertDanger('#commandresult', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_GET_COMMANDRESULT'] != 200) {
        displayAlertDanger('#commandresult', getSvrMsg(responseData['API_GET_COMMANDRESULT']));
        return;
    }

    let resultTableDiv = $('<div id="resulttable" class="table-responsive">');
    if (responseData['API_GET_COMMANDRESULT'].Data === undefined) {
        resultTableDiv.append(getClientMessage('NORESULTEXIST'));
        $('#commandresult').append(resultTableDiv);
    } else {
        let commandresultList = getArray(responseData['API_GET_COMMANDRESULT'].Data.Result);

        let tableListData = $('<table>');
        tableListData.addClass('table stktable table-striped');
    
        let tHead = $('<thead class="thead-light">');
        tHead.append('<tr>' +
                     '<th>' + getClientMessage('RESULT_UPDTIME') + '</th>' + '<th>' + getClientMessage('RESULT_AGTNAME') + '</th>' + '<th>' + getClientMessage('RESULT_CMDNAME') + '</th>' + '<th></th>' +
                     '</tr>');
        tableListData.append(tHead);

        let tBody = $('<tbody>');
        for (let Loop = 0; Loop < commandresultList.length; Loop++) {
            let updTimeInt = parseInt(commandresultList[Loop].UpdTime, 16);
            let dateUpdTime = new Date(updTimeInt * 1000);
            let resultId = commandresultList[Loop].Id;
            tBody.append('<tr><td>' + dateUpdTime + '</td><td>' + commandresultList[Loop].AgentName + '</td><td>' + commandresultList[Loop].CommandName + '</td><td><a id="resultAncId' + resultId + '">' + '<span class="icon icon-terminal" style="font-size:30px;"></span>' + '</a></td></tr>');
        }
        tableListData.append(tBody);

        resultTableDiv.append(tableListData);
        $('#commandresult').append(resultTableDiv);

        for (let loop = 0; loop < commandresultList.length; loop++) {
            let resultId = commandresultList[loop].Id;
            $('#resultAncId' + resultId).on('click', function() {
                transViewConsole(resultId);
            });
        }

    }
    resizeComponent();
}

function transViewConsole(resultId) {
    apiCall('GET', '/api/commandresult/' + resultId + '/', null, 'API_GET_OUTPUT', viewConsole);
}

function viewConsole() {
    let commandOutput = responseData['API_GET_OUTPUT'].Data.Result;
    let consoleDlg = $('<div/>');
    let consoleArea = $('<div style="overflow-wrap: break-word; padding: 6px 6px 10px 10px; color: #ffffff; background-color: #000000; font-family: monospace;"/>');
    consoleArea.html(commandOutput.Output);
    consoleDlg.append(consoleArea);
    consoleDlg.append('<p/>')
    consoleDlg.append('<button type="button" class="btn btn-dark" onclick="closeInputModal();">' + getClientMessage('RESULT_CONSOLECLOSE') + '</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('RESULT_OUTPUT') + '</h5>', consoleDlg);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayDashboard() {
    apiCall('GET', '/api/agent/', null, 'API_GET_AGTINFO', displayDashboard);
}

{
    // Name of selected time series data
    let selectedTsd = new Array('n/a', 'n/a', 'n/a', 'n/a', 'n/a', 'n/a', 'n/a', 'n/a');

    function showSelectTimeSeriesDataDlg() {
        let selectTsdDlg = $('<div/>');
        selectTsdDlg.append(getClientMessage('DASHBOARD_STATUS_DESC'));
    
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        for (let loop = 0; loop < 8; loop++) {
            var btnGrp = $('<div class="btn-group">');
            btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selecteTsd' + loop + '">' + selectedTsd[loop] + '</span><span class="caret"></span></button>');
            var ddMenu = $('<ul class="dropdown-menu" role="menu">');
            ddMenu.append('<li role="presentation"><a onclick="selectTimeSeriesData(' + loop + ', -1)" role="menuitem" tabindex="-1" href="#">' + getClientMessage('DASHBOARD_UNSPECIFIED') + '</a></li>');
            for (let loopMenu = 0; loopMenu < agentInfos.length; loopMenu++) {
                ddMenu.append('<li role="presentation"><a onclick="selectTimeSeriesData(' + loop + ', ' + loopMenu + ')" role="menuitem" tabindex="-1" href="#">' + agentInfos[loopMenu].Name + '</a></li>');
            }
            btnGrp.append(ddMenu);
        
            selectTsdDlg.append('<p>');
            selectTsdDlg.append('' + (loop + 1) + ': ');
            selectTsdDlg.append(btnGrp);
            selectTsdDlg.append('</p>');
        }
    
        showInputModal('<h5 class="modal-title">' + getClientMessage('DASHBOARD_SELECt_AGENT') + '</h5>', selectTsdDlg);
        selectTsdDlg.append('<p>');
        selectTsdDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="changeTimeSeriesData()">OK</button> ');
        selectTsdDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">Cancel</button> ');
        selectTsdDlg.append('</p>');
    }

    function changeTimeSeriesData() {
        closeInputModal();
        displayDashboard();
    }
    
    function selectTimeSeriesData(index, name) {
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (name == -1) {
            selectedTsd[index] = getClientMessage('DASHBOARD_UNSPECIFIED');
        } else {
            selectedTsd[index] = agentInfos[name].Name;
        }
        $('#selecteTsd' + index).text(selectedTsd[index]);
    }

    function displayDashboard() {
        drowContainerFluid($('<div id="dashboard" class="col-xs-12" style="display:block"></div>'));
        if (statusCode['API_GET_AGTINFO'] == -1 || statusCode['API_GET_AGTINFO'] == 0) {
            displayAlertDanger('#dashboard', getClientMessage('CONNERR'));
            return;
        }
        if (statusCode['API_GET_AGTINFO'] != 200) {
            displayAlertDanger('#dashboard', getSvrMsg(responseData['API_GET_AGTINFO']));
            return;
        }
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (agentInfos == null) {
            $('#dashboard').append(getClientMessage('NOAGTINFO'));
            return;
        }
        drasPieChart($('#dashboard'), agentInfos);
    
        $('#dashboard').append('<h4><div class="plane-link">&nbsp;&nbsp;&nbsp;&nbsp;' + getClientMessage('DASHBOARD_AGENTS') + '&nbsp;&nbsp;<a onclick="showSelectTimeSeriesDataDlg();" class="plane-link" style="cursor: pointer;" href="#"><span class="icon icon-cog" style="font-size:20px;"></span></a></div></h4>');
    
        let contents = [];
        for (let loop = 0; loop < 8; loop++) {
            if (selectedTsd[loop] === getClientMessage('DASHBOARD_UNSPECIFIED') || selectedTsd[loop] === 'n/a') {
                continue;
            }
            let tmpUrl = '/api/timeseriesdata/' + selectedTsd[loop] + '/';
            contents.push({ method: 'GET', url: tmpUrl, request: null, keystring: 'API_GET_TIMESERIESDATA_' + loop });
        }
        if (contents.length != 0) {
            initSequentialApiCall();
            sequentialApiCall(contents, drawAgentStatusHistory);
        }
    }

    function drawAgentStatusHistory() {
        finalSequentialApiCall();
        for (let loop = 0; loop < 8; loop++) {
            if (selectedTsd[loop] === getClientMessage('DASHBOARD_UNSPECIFIED') || selectedTsd[loop] === 'n/a') {
                continue;
            }
            let timeseriesdata = getArray(responseData['API_GET_TIMESERIESDATA_' + loop].Data.TimeSeriesData);
            drawAgentStatusHistoryImpl(selectedTsd[loop], timeseriesdata);
        }
    }
    
    function drawAgentStatusHistoryImpl(agentName, timeseriesdata) {
        let wsize = $(window).width();
        let hsize = 70;
    
        let curDate = new Date();
        let curTimeInMs = curDate.getTime();
        let curTime = Math.floor(curTimeInMs / 1000);
        let startTime = curTime - 172800; // 172800 = 60sec * 60min * 24hour * 2days
        let odbTime = curTime - 86400; // 86400 = 60sec * 60min * 24hour
        let unitw = (wsize - 60) / 172800;
        let rectStr = '';
        for (let loopTsd = 0; timeseriesdata != null && loopTsd < timeseriesdata.length; loopTsd++) {
            let theColor = 'LightGreen';
            if (timeseriesdata[loopTsd].Status == 0 ||
                (timeseriesdata[loopTsd].Status <= -980 && timeseriesdata[loopTsd].Status >= -985))
            {
                theColor = 'LightGreen';
            } else {
                theColor = 'LightCoral';
            }
        
            let updTimeInt = parseInt(timeseriesdata[loopTsd].UpdTime, 16);
            if (updTimeInt < startTime) {
                continue;
            }
            let graphX = unitw * (updTimeInt - startTime) + 50;
            let graphWidth = unitw * timeseriesdata[loopTsd].SaInterval + 1;
            if (graphX + graphWidth > wsize - 10) {
                graphWidth = wsize - 10 - graphX;
            }
            let dateUpdTime = new Date(updTimeInt * 1000);
            let label = dateUpdTime + ':' + timeseriesdata[loopTsd].Status;
            rectStr = rectStr + '<rect x="' + graphX + '" y="20" width="' + graphWidth + '" height="30" fill="' + theColor + '"><title>' + label + '</title></rect>';
        }
        let startTimeDate = new Date(startTime * 1000);
        let odbTimeDate = new Date(odbTime * 1000);
    
        let newSvg = $(
            '<svg id="' + agentName + '" xmlns="http://www.w3.org/2000/svg" width="' + (wsize - 10) + '" height="' + hsize + '" viewBox="0 0 ' + (wsize - 10) + ' ' + hsize + '">' +
            '<text x="10" y="19" fill="black">' + agentName + '</text>' +
            '<rect x="50" y="20" width="' + (wsize - 60) + '" height="30" fill="Silver"></rect>' +
            rectStr +
            '<line x1="50" y1="40" x2="50" y2="65" stroke="blue" stroke-width="2"/>' +
            '<text x="53" y="65" fill="blue">' + (startTimeDate.getMonth() + 1) + '/' + startTimeDate.getDate() + ' ' + ('00' + startTimeDate.getHours()).slice(-2) + ':' + ('00' + startTimeDate.getMinutes()).slice(-2) + '</text>' +
            '<line x1="' + ((wsize - 60) / 2 + 50) + '" y1="40" x2="' + ((wsize - 60) / 2 + 50) + '" y2="65" stroke="blue" stroke-width="2"/>' +
            '<text x="' + ((wsize - 60) / 2 + 53) + '" y="65" fill="blue">' + (odbTimeDate.getMonth() + 1) + '/' + odbTimeDate.getDate() + ' ' + ('00' + odbTimeDate.getHours()).slice(-2) + ':' + ('00' + odbTimeDate.getMinutes()).slice(-2) + '</text>' +
            '</svg>'
        );
    
        let escapeSelectorString = function(val){
            return val.replace(/[ !"#$%&'()*+,.\/:;<=>?@\[\\\]^`{|}~]/g, "\\$&");
        }
        
        if ($('#' + escapeSelectorString(agentName)).length) {
            $('#' + escapeSelectorString(agentName)).replaceWith(newSvg);
        } else {
            $('#dashboard').append(newSvg);
        }
    }

    function drasPieChart(element, agentInfos) {
        let totalAgentCnt = agentInfos.length;
        let successCnt = 0;
        let errorCnt = 0;
        let noRequestCnt = 0;
        for (let loop = 0; loop < totalAgentCnt; loop++) {
            if (agentInfos[loop].Status == -993) {
                noRequestCnt++;
            } else if (agentInfos[loop].Status == 0 || (agentInfos[loop].Status <= -980 && agentInfos[loop].Status >= -985)) {
                successCnt++;
            } else {
                errorCnt++;
            }
        }
    
        let errorCntOffset = (successCnt / totalAgentCnt) * 314.15;
        let noRequestCntOffset = errorCntOffset + (errorCnt / totalAgentCnt) * 314.15;
        let wsize = $(window).width();
        let hsize = 220;
     
        let newSvg = $(
            '<svg id="piechart" xmlns="http://www.w3.org/2000/svg" width="' + (wsize - 10) + '" height="' + hsize + '" viewBox="0 0 400 220">' +
            '<text x="250" y="40" font-size="20" fill="black">Total: ' + totalAgentCnt + '</text>' +
            '<text x="250" y="70" font-size="20" fill="black">Success: ' + successCnt + '</text>' +
            '<text x="250" y="100" font-size="20" fill="black">Error: ' + errorCnt + '</text>' +
            '<text x="250" y="130" font-size="20" fill="black">No request: ' + noRequestCnt + '</text>' +
            '<rect x="230" y="55" width="18" height="18" fill="LightGreen"></rect>' +
            '<rect x="230" y="85" width="18" height="18" fill="LightCoral"></rect>' +
            '<rect x="230" y="115" width="18" height="18" fill="Silver"></rect>' +
            '<circle r="50" cx="110" cy="110" fill="rgba(0,0,0,0)" stroke="LightGreen" stroke-width="100" stroke-dashoffset="0" stroke-dasharray="314.15"/>' + 
            '<circle r="50" cx="110" cy="110" fill="rgba(0,0,0,0)" stroke="LightCoral" stroke-width="100" stroke-dashoffset="' + errorCntOffset + '" stroke-dasharray="314.15"/>' +
            '<circle r="50" cx="110" cy="110" fill="rgba(0,0,0,0)" stroke="Silver" stroke-width="100" stroke-dashoffset="' + noRequestCntOffset + '" stroke-dasharray="314.15"/>' +
            '</svg>'
        );
        if ($('#piechart').length) {
            $('#piechart').replaceWith(newSvg);
        } else {
            element.append(newSvg);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function initServal() {
    if (statusCode['API_GET_LANG'] == -1 || statusCode['API_GET_LANG'] == 0) {
        return;
    }
    if (responseData['API_GET_LANG'].Data.ClientLanguage === 'ja') {
        setClientLanguage(1);
    } else {
        setClientLanguage(0);
    }
    showLoginModal(checkLogin);
}

function checkLogin(dummyId, dummyPw) {
    setAuthenticationToken(dummyId + ' ' + dummyPw);
    apiCall('GET', '/api/user/', null, 'API_GET_USER', checkLoginAfterApiCall);
}

function checkLoginAfterApiCall() {
    if (statusCode['API_GET_USER'] == -1 || statusCode['API_GET_USER'] == 0) {
        setLoginResult(2);
        return;
    } else if (statusCode['API_GET_USER'] != 200) {
        setLoginResult(1);
        return;
    } else {
        userRole = responseData['API_GET_USER'].Data.User.Role;

        let contents = [];
        contents = [
            { actApiName : "transDisplayAgentInfo()", title : 'Agent' },
            { actApiName : "transDisplayFileMgmt()", title : "File" },
            { actApiName : "transDisplayCommand()", title : 'Command' },
            { actApiName : "transDisplayCommandResult()", title : 'Result' }
        ];
        iconAlwaysVisible();
        initMainPage('SERVAL', 'squirrel.svg', contents, 'transDisplayDashboard()');
        clearRsCommand();

        let usermenuContents = [];
        if (userRole == 1) {
            usermenuContents = [
                { actApiName: 'transDisplayLogInfo()', title: getClientMessage('OPE_LOG') },
                { actApiName: 'transDisplayChgPassword()', title: getClientMessage('USER_CHG_PW') }
            ];
        } else {
            usermenuContents = [
                { actApiName: 'transDisplayLogInfo()', title: getClientMessage('OPE_LOG') },
                { actApiName: 'transDisplayUser()', title: getClientMessage('USER_MGMT') },
                { actApiName: 'transDisplayChgPassword()', title: getClientMessage('USER_CHG_PW') },
                { actApiName: 'transDisplayServerInfo()', title : getClientMessage('SERVER_INFO') }
            ];
        }
        addRsUserMenu(usermenuContents);
        setLoginResult(0);
        transDisplayDashboard();
        return;
    }
}

function resizeComponent() {
    let wsize = $(window).width();
    let hsize_agentinfotable = $(window).height() - 111;
    let hsize_filemgmttable = $(window).height() - 111;
    let hsize_resulttable = $(window).height() - 57;
    $("#agentinfotable").css("height", hsize_agentinfotable + "px");
    $("#filemgmttable").css("height", hsize_filemgmttable + "px");
    $("#resulttable").css("height", hsize_resulttable + "px");

    // Dashboard resize
    if ($('#dashboard').length) {
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        drasPieChart($('#dashboard'), agentInfos);
        drawAgentStatusHistory();
    }
}

$(document).ready(function () {
    resizeComponent();
});

$(window).resize(function () {
    resizeComponent();
});

window.onload = function() {
    initClientMessage();
    apiCall('GET', '/api/language/', null, 'API_GET_LANG', initServal);
}

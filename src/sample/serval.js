
// User role
var userRole = 0;

// Intervals
var selectedPollingInterval = 1;
var pollingIntervalStr = ['30 sec ', '1 min ', '5 min ', '15 min '];
var selectedStatusAcquisitionInterval = 2;
var statusAcquisitionIntervalStr = ['5 min ', '15 min ', '30 min ', '60 min '];

// Selected user on user management page
var selectedUser = '';

// Selected index number of status acquisition command on agent info page.
var selectedAgentStatusCommand = -1;
// Selected index number of operation command on agent info page.
var selectedOperationCommand = -1;

// Notifications
var svrinfo_msg = '';

function initClientMessage() {
    addClientMessage('USER_MGMT', {'en':'User Management', 'ja':'ユーザー管理'});
    addClientMessage('USER_CHG_PW', {'en':'Change Password', 'ja':'パスワードの変更'});
    addClientMessage('EVENT_LOG', {'en':'Event log', 'ja':'イベントログ'});
    addClientMessage('SERVER_INFO', {'en':'Server info', 'ja':'サーバ情報'});

    addClientMessage('AGENTINFO', {'en':' Agent Info', 'ja':' Agent Info'});
    addClientMessage('SERVERINFO', {'en':' Server Info', 'ja':' Server Info'});
    addClientMessage('COMMAND', {'en':' Command', 'ja':' Command'});

    addClientMessage('NOAGTINFO', {'en':'<p>No agent information</p>', 'ja':'<p>エージェント情報はありません</p>'});
    addClientMessage('NOCMDEXIST', {'en':'<p>No command exists</p>', 'ja':'<p>コマンドはありません</p>'});
    addClientMessage('NOFILEEXIST', {'en':'<p>No file exists</p>', 'ja':'<p>ファイルはありません</p>'});
    addClientMessage('NORESULTEXIST', {'en':'<p>No command result exists</p>', 'ja':'<p>コマンド実行結果はありません</p>'});

    addClientMessage('DASHBOARD_CURRENTSTATUS', {'en':'Current agent status', 'ja':'現在のエージェントの状態'});
    addClientMessage('DASHBOARD_48HSTATUS', {'en':'Status in past 48h', 'ja':'過去48時間の状態'});
    addClientMessage('DASHBOARD_SELECt_AGENT', {'en':'Select agent', 'ja':'エージェントの選択'});
    addClientMessage('DASHBOARD_STATUS_DESC', {'en':'You can choose maximum 8 agents time series status on dashboard.', 'ja':'最大8件のエージェントの状態をダッシュボードに表示できます。'});
    addClientMessage('DASHBOARD_UNSPECIFIED', {'en':'Unspecified', 'ja':'未指定'});
    addClientMessage('DASHBOARD_STEP', {
        'en':'Servers and IoT devices can be managed by following steps.',
        'ja':'次のステップで、サーバおよびIoTデバイスを管理することができます。'
    });
    addClientMessage('DASHBOARD_STEP1', {
        'en':'After install agent to management target, edit properties file and configure destination setting, then restart the agent. ' +
             'New agent is appeared on "Agent" page after the notification.',
        'ja':'管理対象にエージェントをインストール後、プロパティファイルを編集し、通知先の設定を行い、エージェントを再起動します。' +
             '通知後"Agent"ページに新規のエージェントが追加されます。'
    });
    addClientMessage('DASHBOARD_STEP2', {
        'en':'Creates a command for monitoring and management and configure it to agent.',
        'ja':'監視・管理用のコマンドを作成し、エージェントに設定してください。'
    });
    addClientMessage('DASHBOARD_STEP3', {
        'en':'You can refer to the command execution result on "Result" page.',
        'ja':'コマンドの実行結果は"Result"ページから参照できます。'
    });

    addClientMessage('AINAME', {'en':'Name', 'ja':'名称'});
    addClientMessage('AISTATUS', {'en':'Status', 'ja':'状態'});
    addClientMessage('AISTATUSCMD', {'en':'Command for Status', 'ja':'状態取得コマンド'});
    addClientMessage('AISTATUSINITIME', {'en':'Initial Notification Time', 'ja':'初回通知時刻'});
    addClientMessage('AILASTPOLLINGTIME', {'en':'Last Polling Time', 'ja':'最終ポーリング時刻'});
    addClientMessage('AISTATUSTIME', {'en':'Status Acquisition Time', 'ja':'状態取得時刻'});
    addClientMessage('AISTATUSUPTIME', {'en':'Status Update Time', 'ja':'状態更新時刻'});
    addClientMessage('AIOPSTATUS', {'en':'OpStatus', 'ja':'操作状態'});
    addClientMessage('AIOPSTATUSCMD', {'en':'Command for Op', 'ja':'操作コマンド'});
    addClientMessage('AIOPEXECTIME', {'en':'Operation Execution Time', 'ja':'操作実行時刻'});
    addClientMessage('AISETSTATUSCMD', {'en':'Select command', 'ja':'コマンド設定'});
    addClientMessage('AIEXECCMD', {'en':'Execute command', 'ja':'コマンド実行'});
    addClientMessage('SELOPCMD', {'en':'Select operation command', 'ja':'操作コマンドを選択してください'});
    addClientMessage('SELSTATUSCMD', {'en':'Select status acquisition command', 'ja':'状態取得コマンドを選択してください'});
    addClientMessage('SELCMD', {'en':'Select Command ', 'ja':'コマンドを選択してください'});
    addClientMessage('AICMDUNSPECIFIED', {'en':'Unspecified', 'ja':'未指定'});
    addClientMessage('DELAGENT', {'en':'Delete agents', 'ja':'エージェントの削除'});
    addClientMessage('DELAGENTCONFIRM', {
        'en':'Information related wtih the specified agents will be deleted from SERVAL server .<br/>Uninstall agents from the hosts to avoid re-registration of the agents.<br/>', 
        'ja':'指定したエージェントに関連する情報はSERVALサーバから削除されます。<br/>エージェントが再登録されないようにホストからエージェントをアンインストールしてください。<br/>'
    });
    addClientMessage('DELAGENTLIST', {'en':'Agents shown below will be deleted.', 'ja':'以下のエージェントが削除されます。'});
    addClientMessage('AIDOESNOTEXIST', {'en':'The specified agent does not exist.', 'ja':'指定したエージェントは存在しません。'});
    addClientMessage('RESCODE_SUC', {'en':'SUCCS : Command successfully ended.\r\n', 'ja':'SUCCS : コマンドが成功した\r\n'});
    addClientMessage('RESCODE_FAI', {'en':'FAILD : Command failed.\r\n', 'ja':'FAILD : コマンドが失敗した\r\n'});
    addClientMessage('RESCODE-970', {'en':'NOREQ : No request from agent.\r\n', 'ja':'NOREQ : エージェントからの要求がない\r\n'});
    addClientMessage('RESCODE-980', {'en':'START : Agent service has started.\r\n', 'ja':'START : エージェントサービスが起動した\r\n'});
    addClientMessage('RESCODE-981', {'en':'NOCMD : No status acquisition command is configured.\r\n', 'ja':'NOCMD : 状態取得コマンドが設定されていない\r\n'});
    addClientMessage('RESCODE-982', {'en':'STCHG : Status acquisition command has been changed.\r\n', 'ja':'STCHG : 状態取得コマンドが変更された\r\n'});
    addClientMessage('RESCODE-983', {'en':'OPCHG : Operation command has been changed.\r\n', 'ja':'OPCHG : 操作コマンドが変更された\r\n'});
    addClientMessage('RESCODE-984', {'en':'OPWIT : Waiting for an operation command request.\r\n', 'ja':'OPWIT : 操作コマンド要求待ち\r\n'});
    addClientMessage('RESCODE-985', {'en':'OPEXE : A command for operation has started.\r\n', 'ja':'OPEXE : 操作コマンドが実行された\r\n'});
    addClientMessage('RESCODE-990', {'en':'SFILE : Server file handling error.\r\n', 'ja':'SFILE : サーバファイルハンドリングエラー\r\n'});
    addClientMessage('RESCODE-991', {'en':'AFILE : Agent file handling error.\r\n', 'ja':'AFILE : エージェントファイルハンドリングエラー\r\n'});
    addClientMessage('RESCODE-992', {'en':'PLATF : Platform error.\r\n', 'ja':'PLATF : プラットフォームエラー\r\n'});
    addClientMessage('RESCODE-993', {'en':'TIMEO : Timeout error.\r\n', 'ja':'TIMEO : タイムアウトエラー\r\n'});
    addClientMessage('RESCODE-994', {'en':'AGDIR : Invalid agent directory.\r\n', 'ja':'AGDIR : エージェントディレクトリが不正\r\n'});
    addClientMessage('RESCODE-995', {'en':'CMRLT : Command execution result handling error.\r\n', 'ja' : 'CMRLT : コマンド実行結果ハンドリングエラー\r\n'});

    addClientMessage('SISTARTTIME', {'en':'Service start time : ', 'ja':'サービス開始時刻 : '});
    addClientMessage('SISVRVERSION', {'en':'Server version : ', 'ja':'サーバーバージョン : '});
    addClientMessage('SIPOLLINTVL', {'en':'Polling interval : ', 'ja':'ポーリング間隔 : '});
    addClientMessage('SISTACQINTVL', {'en':'Status acquisition interval : ', 'ja':'状態取得間隔 : '});
    addClientMessage('SIUPDATEBTN', {'en':'Update', 'ja':'更新'});
    addClientMessage('SICLOSE', {'en':'Close', 'ja':'閉じる'});
    addClientMessage('SIUPDATED', {'en':'The server information has been updated.', 'ja':'サーバー情報が更新されました。'});
    addClientMessage('SIBUILDTIME', {'en':'Service build time : ', 'ja':'サービスビルド時刻 : '});
    addClientMessage('SIPHYMEM', {'en':'Used physical memory size in KB : ', 'ja':'使用物理メモリサイズ(KB) : '});
    addClientMessage('SIVIRMEM', {'en':'Used virtual memory size in KB : ', 'ja':'使用仮想メモリサイズ(KB) : '});

    addClientMessage('FILE_UPLOAD', {'en':'Upload', 'ja':'アップロード'});
    addClientMessage('FILE_DELETE', {'en':'Delete', 'ja':'削除'});
    addClientMessage('FILE_NAME', {'en':'File name', 'ja':'ファイル名'});
    addClientMessage('FILE_SIZE', {'en':'File size', 'ja':'ファイルサイズ'});
    addClientMessage('FILE_UPDATE_TIME', {'en':'Update time', 'ja':'更新時刻'});
    addClientMessage('FILE_DEL', {'en':'Delete File', 'ja':'ファイルの削除'});
    addClientMessage('FILE_DELCONFIRM', {'en':'Are you sure you want to delete the specified file(s)?', 'ja':'指定したファイルを削除します。'});

    addClientMessage('COMNAME', {'en':'Command Name', 'ja':'コマンド名'});
    addClientMessage('COMFILESTOAGT', {'en':'File(s) Copied To Agent', 'ja':'エージェントにコピーされるファイル'});
    addClientMessage('COMFILESTOSVR', {'en':'File(s) Copied To Server', 'ja':'サーバーにコピーされるファイル'});
    addClientMessage('COMSCRIPTDEF', {'en':'Script Definition', 'ja':'スクリプト定義'});
    addClientMessage('COMCOPYTOAGT', {'en':'File To Be Copied To Agent', 'ja':'エージェントにコピーされるファイル'});
    addClientMessage('COMTYPE', {'en':'Command Type', 'ja':'コマンド種別'});
    addClientMessage('COMSCRIPT', {'en':'Script', 'ja':'スクリプト'});
    addClientMessage('COMTIMEOUT', {'en':'Script Timeout Intereval (Sec)', 'ja':'スクリプトのタイムアウト時間 (秒)'});
    addClientMessage('COMCOPYTOSVR', {'en':'File To Be Copied To Server (Only file name. Do not specify directory path.)', 'ja':'サーバーにコピーされるファイル (ファイル名のみ)'});
    addClientMessage('COMPLACEAGT', {'en':'Name of file Placed In Agent', 'ja':'エージェントに配置されたファイルの名称'});
    addClientMessage('COMADD', {'en':'Add', 'ja':'追加'});
    addClientMessage('COMUPDATE', {'en':'Update', 'ja':'更新'});
    addClientMessage('COMDELETE', {'en':'Delete', 'ja':'削除'});
    addClientMessage('COMCANCEL', {'en':'Cancel', 'ja':'キャンセル'});
    addClientMessage('COMCLOSE', {'en':'Close', 'ja':'閉じる'});
    addClientMessage('COMADDCOMMAND', {'en':'Add Command', 'ja':'コマンド追加'});
    addClientMessage('COMEDITCOMMAND', {'en':'Edit Command', 'ja':'コマンド編集'});
    addClientMessage('COMDELCOMMAND', {'en':'Delete Command', 'ja':'コマンドの削除'});
    addClientMessage('COMDELCONFIRM', {'en':'Are you sure you want to delete the specified command(s)?', 'ja':'指定したコマンドを削除します。'});
    addClientMessage('COMMANDLABEL', {'en':'Command : ', 'ja':'コマンド : '});
    addClientMessage('COMMANDEXITCODE', {'en':'Exit code=0 : Success, Otherwise : Failed', 'ja':'Exit code=0 : Success, Otherwise : Failed'});

    addClientMessage('RESULT_UPDTIME', {'en':'Execution date and time', 'ja':'実行日時'});
    addClientMessage('RESULT_AGTNAME', {'en':'Agent', 'ja':'エージェント'});
    addClientMessage('RESULT_CMDNAME', {'en':'Command', 'ja':'コマンド'});
    addClientMessage('RESULT_RESULT', {'en':'Execution Result', 'ja':'実行結果'});
    addClientMessage('RESULT_EXITCODE', {'en':'Exit code', 'ja':'Exit code'});
    addClientMessage('RESULT_CONSOLECLOSE', {'en':'Close', 'ja':'閉じる'});
    addClientMessage('RESULT_TITLE', {'en':'Command execution result', 'ja':'コマンド実行結果'});

    addClientMessage('CONNERR', {
        'en':'Connection with REST API service failed. This may be caused by one of the following issues:<br>(1) REST API service cannot be started.<br>(2) REST API service is not registered as a firewall exception.<br>(3) The definition file [nginx.conf and/or sample.conf] for the host name and port number in the network connectivity settings is invalid.<br>(4) A timeout has occurred when waiting for data from REST API server.<br>',
        'ja':'REST APIサービスとの通信が失敗しました。次の要因が考えられます。<br>(1) REST APIサービスが開始されていない。<br>(2) REST APIサービスがファイアウォールに例外登録されていない。<br>(3) 接続先ホスト名およびポート番号の定義ファイル [nginx.conf , sample.conf] が不正。<br>(4) REST APIサーバからのデータ取得中にタイムアウトが発生した。<br>'
    });
    addClientMessage('UNEXPECTEDERR', {'en':'Unexpected error occurred.', 'ja':'予期しないエラーが発生しました。'});
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

function commonErrorHandling(element, errorCode) {
    if (statusCode[errorCode] === undefined || responseData[errorCode] === undefined) {
        displayAlertDanger(element, getClientMessage('UNEXPECTEDERR'));
        return -1;
    }
    if (statusCode[errorCode] == -1 || statusCode[errorCode] == 0) {
        displayAlertDanger(element, getClientMessage('CONNERR'));
        return -1;
    }
    if (statusCode[errorCode] != 200) {
        displayAlertDanger(element, getSvrMsg(responseData[errorCode]));
        return -1;
    }
    return 0;
}

function getStatusLabel(status) {
    switch (status) {
        case -970: return 'NOREQ';
        case -980: return 'START';
        case -981: return 'NOCMD';
        case -982: return 'STCHG';
        case -983: return 'OPCHG';
        case -984: return 'OPWIT';
        case -985: return 'OPEXE';
        case -990: return 'SFILE';
        case -991: return 'AFILE';
        case -992: return 'PLATF';
        case -993: return 'TIMEO';
        case -994: return 'AGDIR';
        case -995: return 'CMRLT';
        case 0: return 'SUCCS';
        default: return 'FAILD';
    }
}

function getStatusDetailLabel(status) {
    switch (status) {
        case -970: return getClientMessage('RESCODE-970');
        case -980: return getClientMessage('RESCODE-980');
        case -981: return getClientMessage('RESCODE-981');
        case -982: return getClientMessage('RESCODE-982');
        case -983: return getClientMessage('RESCODE-983');
        case -984: return getClientMessage('RESCODE-984');
        case -985: return getClientMessage('RESCODE-985');
        case -990: return getClientMessage('RESCODE-990');
        case -991: return getClientMessage('RESCODE-991');
        case -992: return getClientMessage('RESCODE-992');
        case -993: return getClientMessage('RESCODE-993');
        case -994: return getClientMessage('RESCODE-994');
        case -995: return getClientMessage('RESCODE-995');
        case 0: return getClientMessage('RESCODE_SUC');
        default: return getClientMessage('RESCODE_FAI');
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
                 '<th class="d-none d-lg-table-cell">' + getClientMessage('AISTATUSUPTIME') + '</th>' +
                 '<th>' + getClientMessage('AIOPSTATUS') + '</th>' +
                 '<th class="d-none d-sm-table-cell">' + getClientMessage('AIOPSTATUSCMD') + '</th>' +
                 '<th class="d-none d-lg-table-cell">' + getClientMessage('AIOPEXECTIME') + '</th>' +
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
        let updTimeStr = '';
        if (AgentInfo[Loop].UpdTime != 0) {
            updTimeStr = getDateAndTimeStr(AgentInfo[Loop].UpdTime);
        }
        let opeTimeStr = '';
        if (AgentInfo[Loop].OpeTime != 0) {
            opeTimeStr = getDateAndTimeStr(AgentInfo[Loop].OpeTime);
        }

        let tmpChkBoxStr = '';
        if (userRole != 1) {
            tmpChkBoxStr = '<td><div class="checkbox"><input type="checkbox" id="agtInfoId' + Loop + '" value="" onclick="switchAgentInfoButton()"/></div></td>';
        }
        tBody.append('<tr>' +
                     tmpChkBoxStr +
                     '<td><a id="agentprop' + Loop + '" style="cursor: pointer;"><u>' + AgentInfo[Loop].Name + '</u></a></td>' +
                     '<td><div align="center" id="statusTd' + Loop + '" data-toggle="tooltip" title="' + getTooltipStr() + '">' + getStatusLabel(AgentInfo[Loop].Status) + '</div></td>' +
                     '<td class="d-none d-sm-table-cell">' + cmdNameStatus + '</td>' +
                     '<td class="d-none d-lg-table-cell">' + updTimeStr + '</td>' +
                     '<td><div align="center" id="opStatusTd' + Loop + '" data-toggle="tooltip" title="' + getTooltipStr() + '">' + getStatusLabel(AgentInfo[Loop].OpStatus) + '</div></td>' +
                     '<td class="d-none d-sm-table-cell">' + cmdNameOp + '</td>' +
                     '<td class="d-none d-lg-table-cell">' + opeTimeStr + '</td>' +
                     '</tr>');
        // Draw rectangle for agent status
        if (AgentInfo[Loop].Status == 0) {
            $('#statusTd' + Loop).css('background-color', 'LightGreen');
        } else if (AgentInfo[Loop].Status <= -970 && AgentInfo[Loop].Status >= -979) {
            $('#statusTd' + Loop).css('background-color', 'Silver');
        } else if (AgentInfo[Loop].Status <= -980 && AgentInfo[Loop].Status >= -989) {
            $('#statusTd' + Loop).css('background-color', 'LightSkyBlue');
        } else {
            $('#statusTd' + Loop).css('background-color', 'LightCoral');
        }
        // Draw rectangle for operation status
        if (AgentInfo[Loop].OpStatus == 0) {
            $('#opStatusTd' + Loop).css('background-color', 'LightGreen');
        } else if (AgentInfo[Loop].OpStatus <= -980 && AgentInfo[Loop].OpStatus >= -989) {
            $('#opStatusTd' + Loop).css('background-color', 'LightSkyBlue');
        } else {
            $('#opStatusTd' + Loop).css('background-color', 'LightCoral');
        }
    }
    for (let loopclick = 0; loopclick < AgentInfo.length; loopclick++) {
        $('#agentprop' + loopclick).on('click', function() {
            transShowAgentPropertiesDialog(AgentInfo[loopclick].Name);
        });
    }
    resizeComponent();
}

function getTooltipStr() {
    var tooltipStr = getClientMessage('RESCODE_SUC') +
                     getClientMessage('RESCODE_FAI') +
                     getClientMessage('RESCODE-970') +
                     getClientMessage('RESCODE-980') +
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
    clearRsCommand();
    if (foundFlag == true) {
        addRsCommand("displayAgentStatusCommandDlg()", "icon-pencil", true);
        addRsCommand("displayExecCommandDlg()", "icon-play", true);
        addRsCommand("displayDeleteAgentDlg()", "icon-bin", true);
    } else {
        addRsCommand("displayAgentStatusCommandDlg()", "icon-pencil", false);
        addRsCommand("displayExecCommandDlg()", "icon-play", false);
        addRsCommand("displayDeleteAgentDlg()", "icon-bin", false);
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
    execCommandDlg.append(getClientMessage('COMMANDLABEL'));
    execCommandDlg.append(btnGrp);
    execCommandDlg.append('<p></p>');

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
    ddMenu.append('<li role="presentation"><a onclick="selectAgentStatusCommand(-1)" role="menuitem" tabindex="-1" href="#">' + getClientMessage('AICMDUNSPECIFIED') + '</a></li>');
    for (var loop = 0; commandList != null && loop < commandList.length; loop++) {
        ddMenu.append('<li role="presentation"><a onclick="selectAgentStatusCommand(' + loop + ')" role="menuitem" tabindex="-1" href="#">' + commandList[loop].Name + '</a></li>');
    }
    btnGrp.append(ddMenu);
    execSaCommandDlg.append(getClientMessage('COMMANDLABEL'));
    execSaCommandDlg.append(btnGrp);
    execSaCommandDlg.append('<p></p>');

    execSaCommandDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeAgentStatusCommandDlg(true)">OK</button> ');
    execSaCommandDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeAgentStatusCommandDlg(false)">Cancel</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('SELSTATUSCMD') + '</h5>', execSaCommandDlg);
}

function closeAgentStatusCommandDlg(okFlag) {
    if (okFlag == true) {
        var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        var AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        for (var loop = 0; loop < AgentInfo.length; loop++) {
            if ($('#agtInfoId' + loop).prop('checked') == true) {
                var ReqObj = {};
                if (selectedAgentStatusCommand == -1) {
                    ReqObj = { Name : AgentInfo[loop].Name, StatusCmd : -1 };
                } else {
                    ReqObj = { Name : AgentInfo[loop].Name, StatusCmd : commandList[selectedAgentStatusCommand].Id };
                }
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
    var commandId = -999;
    var commandStr = '';
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            if (commandId != -999 && AgentInfo[loop].StatusCmd != commandId) {
                allSame = false;
            } else {
                commandId = AgentInfo[loop].StatusCmd;
            }
        }
    }
    if (allSame == false) {
        return '';
    } else {
        if (commandId == -1) {
            return getClientMessage('AICMDUNSPECIFIED');
        } else if (commandList != null) {
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
    if (agentStatusCommand == -1) {
        $('#selectedAgentStatus').text(getClientMessage('AICMDUNSPECIFIED'));
    } else {
        var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        $('#selectedAgentStatus').text(commandList[agentStatusCommand].Name);
    }
    selectedAgentStatusCommand = agentStatusCommand;
}

function displayDeleteAgentDlg() {
    let AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    let foundFlag = false;
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            foundFlag = true;
        }
    }
    if (foundFlag == false) {
        return;
    }

    let deleteAgentDlg = $('<div/>')
    deleteAgentDlg.append(getClientMessage('DELAGENTCONFIRM'));
    deleteAgentDlg.append(getClientMessage('DELAGENTLIST'));
    deleteAgentDlg.append('<p></p>');
    for (let loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            deleteAgentDlg.append('&nbsp;&nbsp;&nbsp;' + AgentInfo[loop].Name + '<br/>');
        }
    }
    deleteAgentDlg.append('<p></p>');

    deleteAgentDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeDeleteAgentDlg()">Delete</button> ');
    deleteAgentDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">Cancel</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('DELAGENT') + '</h5>', deleteAgentDlg);
}

function closeDeleteAgentDlg() {
    let AgentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
    let contents = [];
    for (var loop = 0; loop < AgentInfo.length; loop++) {
        if ($('#agtInfoId' + loop).prop('checked') == true) {
            contents.push({ method: 'DELETE', url: '/api/agent/' + AgentInfo[loop].Name + '/', request: null, keystring: 'API_DELETE_AGTINFO' })
        }
    }
    initSequentialApiCall();
    sequentialApiCall(contents, completeDeleteAgentDlg);
    closeInputModal();
}

function completeDeleteAgentDlg() {
    finalSequentialApiCall();

    let errDlg = function(msg) {
        let deleteAgentDlg = $('<div/>');
        showInputModal('<h5 class="modal-title">' + getClientMessage('DELAGENT') + '</h5>', deleteAgentDlg);
        deleteAgentDlg.append(msg);
        deleteAgentDlg.append('<p></p>');
        deleteAgentDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeInputModal()">OK</button> ');
        transDisplayAgentInfo();
    }

    if (statusCode['API_DELETE_AGTINFO'] == -1 || statusCode['API_DELETE_AGTINFO'] == 0) {
        errDlg(getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_DELETE_AGTINFO'] != 200) {
        errDlg(getSvrMsg(responseData['API_DELETE_AGTINFO']));
        return;
    }
    transDisplayAgentInfo();
}

{
    let targetName = '';

    function transShowAgentPropertiesDialog(paramTargetName) {
        targetName = paramTargetName;
        let contents = [];
        contents.push({ method: 'GET', url: '/api/timeseriesdata/' + targetName + '/', request: null, keystring: 'API_GET_TIMESERIESDATA' });
        contents.push({ method: 'GET', url: '/api/agent/', request: null, keystring: 'API_GET_AGTINFO' });
        MultiApiCall(contents, showAgentPropertiesDialog);
    }
    
    function resizeAgentStatusHistory(wsize) {
        let escapeSelectorString = function(val){
            return val.replace(/[ !"#$%&'()*+,.\/:;<=>?@\[\\\]^`{|}~]/g, "\\$&");
        }
        let timeseriesdata = getArray(responseData['API_GET_TIMESERIESDATA'].Data.TimeSeriesData);
        resetCurDate();
        let newSvg = drawAgentStatusHistoryImpl(targetName, timeseriesdata, wsize);
        if ($('#' + escapeSelectorString(targetName)).length) {
            $('#' + escapeSelectorString(targetName)).replaceWith(newSvg);
        } else {
            $('#agentdetail').append(newSvg);
        }
    }

    function showAgentPropertiesDialog() {
        let errDlg = function(msg) {
            let errDlgDiv = $('<div/>');
            showInputModal('<h5 class="modal-title">' + targetName + '</h5>', errDlgDiv);
            errDlgDiv.append(msg);
            errDlgDiv.append('<p></p>');
            errDlgDiv.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeInputModal()">OK</button> ');
            transDisplayAgentInfo();
        }

        if (statusCode['API_GET_AGTINFO'] == -1 || statusCode['API_GET_AGTINFO'] == 0) {
            errDlg(getClientMessage('CONNERR'));
            return;
        }
        if (statusCode['API_GET_AGTINFO'] != 200) {
            errDlg(getSvrMsg(responseData['API_GET_AGTINFO']));
            return;
        }
        if (responseData['API_GET_AGTINFO'].Data === undefined) {
            errDlg(getClientMessage('NOAGTINFO'));
            return;
        }
        let agentInfo = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (agentInfo == null) {
            errDlg(getClientMessage('NOAGTINFO'));
            return;
        }
        let targetAgentExistFlag = false;
        for (let loop = 0; loop < agentInfo.length; loop++) {
            if (targetName === agentInfo[loop].Name) {
                let agentPropDlg = $('<div id="agentdetail"/>');
                showInputModal('<h5 class="modal-title">' + targetName + '</h5>', agentPropDlg);

                targetAgentExistFlag = true;
                resizeAgentStatusHistory(320);
                agentPropDlg.append('<p></p>');
       
                let tableListData = $('<table>');
                tableListData.addClass('table stktable table-striped');
                let tBody = $('<tbody>');
                tableListData.append(tBody);
                agentPropDlg.append(tableListData);

                let acqTimeStr = '';
                if (agentInfo[loop].AcqTime != 0) {
                    acqTimeStr = getDateAndTimeStr(agentInfo[loop].AcqTime);
                }
                let updTimeStr = '';
                if (agentInfo[loop].UpdTime != 0) {
                    updTimeStr = getDateAndTimeStr(agentInfo[loop].UpdTime);
                }
                let iniTimeStr = getDateAndTimeStr(agentInfo[loop].IniTime);
                let reqTimeStr = getDateAndTimeStr(agentInfo[loop].ReqTime);
                let opeTimeStr = '';
                if (agentInfo[loop].OpeTime != 0) {
                    opeTimeStr = getDateAndTimeStr(agentInfo[loop].OpeTime);
                }

                let commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
                let cmdNameStatus = '';
                let cmdNameOpStatus = '';
                for (let loopcmd = 0; commandList != null && loopcmd < commandList.length; loopcmd++) {
                    if (agentInfo[loop].StatusCmd == commandList[loopcmd].Id) {
                        cmdNameStatus = commandList[loopcmd].Name;
                    }
                    if (agentInfo[loop].OpCmd == commandList[loopcmd].Id) {
                        cmdNameOpStatus = commandList[loopcmd].Name;
                    }
                }
        
                tBody.append('<tr><td>' + getClientMessage('AISTATUS') + '</td><td id="adStatusTd' + loop + '">' + getStatusDetailLabel(agentInfo[loop].Status) + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AISTATUSCMD') + '</td><td>' + cmdNameStatus + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AISTATUSTIME') + '</td><td>' + acqTimeStr + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AISTATUSUPTIME') + '</td><td>' + updTimeStr + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AISTATUSINITIME') + '</td><td>' + iniTimeStr + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AILASTPOLLINGTIME') + '</td><td>' + reqTimeStr + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AIOPSTATUS') + '</td><td id="adOpStatusTd' + loop + '">' + getStatusDetailLabel(agentInfo[loop].OpStatus) + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AIOPSTATUSCMD') + '</td><td>' + cmdNameOpStatus + '</td></tr>');
                tBody.append('<tr><td>' + getClientMessage('AIOPEXECTIME') + '</td><td>' + opeTimeStr + '</td></tr>');
                if (agentInfo[loop].Status == 0) {
                    $('#adStatusTd' + loop).css('background-color', 'LightGreen');
                } else if (agentInfo[loop].Status <= -970 && agentInfo[loop].Status >= -979) {
                    $('#adStatusTd' + loop).css('background-color', 'Silver');
                } else if (agentInfo[loop].Status <= -980 && agentInfo[loop].Status >= -989) {
                    $('#adStatusTd' + loop).css('background-color', 'LightSkyBlue');
                } else {
                    $('#adStatusTd' + loop).css('background-color', 'LightCoral');
                }
                if (agentInfo[loop].OpStatus == 0) {
                    $('#adOpStatusTd' + loop).css('background-color', 'LightGreen');
                } else if (agentInfo[loop].OpStatus <= -970 && agentInfo[loop].OpStatus >= -979) {
                    $('#adOpStatusTd' + loop).css('background-color', 'Silver');
                } else if (agentInfo[loop].OpStatus <= -980 && agentInfo[loop].OpStatus >= -989) {
                    $('#adOpStatusTd' + loop).css('background-color', 'LightSkyBlue');
                } else {
                    $('#adOpStatusTd' + loop).css('background-color', 'LightCoral');
                }
                agentPropDlg.append('<p></p>');
                agentPropDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="closeInputModal()">OK</button> ');

                resizeComponent();
            }
        }
        if (targetAgentExistFlag == false) {
            errDlg(getClientMessage('AIDOESNOTEXIST'));
            return;
        }
    }    
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayServerInfo() {
    apiCall('GET', '/api/server/', null, 'API_GET_SVRINFO', displayServerInfo);
}

function displayServerInfo() {
    let serverInfoDlg = $('<div id="serverinfo"/>')
    showInputModal('<h5 class="modal-title">' + getClientMessage('SERVERINFO') + '</h5>', serverInfoDlg);

    if (statusCode['API_GET_SVRINFO'] == -1 || statusCode['API_GET_SVRINFO'] == 0) {
        displayAlertDanger("#serverinfo", getClientMessage('CONNERR'));
        serverInfoDlg.append('<p></p>');
        serverInfoDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('SICLOSE') + '</button> ');
        return;
    }
    if (statusCode['API_GET_SVRINFO'] != 200) {
        displayAlertDanger("#serverinfo", getSvrMsg(responseData['API_GET_SVRINFO']));
        serverInfoDlg.append('<p></p>');
        serverInfoDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('SICLOSE') + '</button> ');
        return;
    }

    serverInfoDlg.append('<p>' + getClientMessage('SISVRVERSION') + responseData['API_GET_SVRINFO'].Data.ServerInfo.Version + '</p>');

    let buildTimeStr = responseData['API_GET_SVRINFO'].Data.ServerInfo.BuildTime;
    serverInfoDlg.append('<p>' + getClientMessage('SIBUILDTIME') + buildTimeStr + '</p>');

    let StartTimeStr = getDateAndTimeStr(responseData['API_GET_SVRINFO'].Data.ServerInfo.StartTime);
    serverInfoDlg.append('<p>' + getClientMessage('SISTARTTIME') + StartTimeStr + '</p>');

    let PhyMem = responseData['API_GET_SVRINFO'].Data.ServerInfo.UsedPhysicalMemory;
    let VirMem = responseData['API_GET_SVRINFO'].Data.ServerInfo.UsedVirtualMemory;
    serverInfoDlg.append('<p>' + getClientMessage('SIPHYMEM') + PhyMem + '</p>');
    serverInfoDlg.append('<p>' + getClientMessage('SIVIRMEM') + VirMem + '</p>');

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
    serverInfoDlg.append('<p></p>');
    serverInfoDlg.append(getClientMessage('SIPOLLINTVL'));
    serverInfoDlg.append(btnGrp);
    serverInfoDlg.append('<p></p>');

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

    serverInfoDlg.append('<p></p>');
    serverInfoDlg.append(getClientMessage('SISTACQINTVL'));
    serverInfoDlg.append(btnGrp);
    serverInfoDlg.append('<p></p>');
    
    serverInfoDlg.append('<div id="svrinfo_msg"/>');

    serverInfoDlg.append('<p></p>');
    serverInfoDlg.append('<button type="button" id="serverInfoBtnUpdate" class="btn btn-dark" onclick="updateServerInfo()">' + getClientMessage('SIUPDATEBTN') + '</button> ');
    serverInfoDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('SICLOSE') + '</button> ');
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
        displayAlertDanger('#svrinfo_msg', getClientMessage('CONNERR'));
        return;
    }
    if (statusCode['API_POST_SVRINFO'] != 200) {
        displayAlertDanger('#svrinfo_msg', getSvrMsg(responseData['API_POST_SVRINFO']));
        return;
    }
    displayAlertSuccess('#svrinfo_msg', getClientMessage('SIUPDATED'));
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

            let dateUpdTime = getDateAndTimeStr(fileList[Loop].UpdTime);
            tBody.append('<tr>' + tmpChkBoxStr + '<td><a id="fileInfoAncId' + Loop + '" style="cursor: pointer;"><u>' + fileList[Loop].Name + '</u></a></td><td>' + fileList[Loop].Size + '</td><td>' + dateUpdTime + '</td></tr>');
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

    $('#filemgmt').append('<form id="fileuploadform" method="post" action="/api/filea/" enctype="multipart/form-data"><input type="file" id="fileupload" style="display:none" multiple/></form>');
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
                        if (loop % 100000 == 99999 || loop == orgData.byteLength - 1) {
                            let offset = 0;
                            if (loop < 100000) {
                                offset = 0;
                            } else {
                                offset = parseInt(loop / 100000) * 100000;
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
    let chunks = parseInt(filesize / 100000 + 1);
    let contents = [];
    for (let loop = 0; loop < chunks; loop++) {
        let encFileName = encodeURI(fileName);
        let tmpUrl = '/api/file/' + encFileName + '/' + loop * 100000 + '/';
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
    let chunks = parseInt(fileSize / 100000 + 1);
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

function clickUpload() {
    $("#fileupload").trigger("click");
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
    clearRsCommand();
    if  (userRole != 1) {
        addRsCommand("clickUpload()", "icon-cloud-upload", true);
    } else {
        addRsCommand("clickUpload()", "icon-cloud-upload", false);
    }
    if (foundFlag == true) {
        addRsCommand("displayDeleteFileDlg()", "icon-bin", true);
    } else {
        addRsCommand("displayDeleteFileDlg()", "icon-bin", false);
    }
}

function displayDeleteFileDlg() {
    let fileInfo = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);
    let foundFlag = false;
    for (var loop = 0; fileInfo != null && loop < fileInfo.length; loop++) {
        if ($('#fileInfoId' + loop).prop('checked') == true) {
            foundFlag = true;
        }
    }
    if (foundFlag == false) {
        return;
    }

    let deleteFileDlg = $('<div/>')
    deleteFileDlg.append(getClientMessage('FILE_DELCONFIRM'));
    deleteFileDlg.append('<p></p>');
    for (let loop = 0; fileInfo != null && loop < fileInfo.length; loop++) {
        if ($('#fileInfoId' + loop).prop('checked') == true) {
            let encFileName = encodeURI(fileInfo[loop].Name);
            deleteFileDlg.append('&nbsp;&nbsp;&nbsp;' + encFileName + '<br/>');
        }
    }
    deleteFileDlg.append('<p></p>');
    deleteFileDlg.append('<div id="file_errmsg"/>');
    deleteFileDlg.append('<p></p>');

    deleteFileDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="deleteFile()">Delete</button> ');
    deleteFileDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">Cancel</button> ');

    showInputModal('<h5 class="modal-title">' + getClientMessage('FILE_DEL') + '</h5>', deleteFileDlg);
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
            displayAlertDanger('#file_errmsg', getClientMessage('CONNERR'));
            errorFlag = true;
            break;
        }
        if (statusCode['API_DELETE_FILE' + loop] != 200) {
            displayAlertDanger('#file_errmsg', getSvrMsg(responseData['API_DELETE_FILE' + loop]));
            errorFlag = true;
            break;
        }
    }
    for (var loop = 0; loop < fileList.length; loop++) {
        delete responseData['API_DELETE_FILE' + loop];
        delete statusCode['API_DELETE_FILE' + loop];
    }
    if (errorFlag == false) {
        closeInputModal();
        transDisplayFileMgmt();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

function transDisplayCommand() {
    let contents = [];
    contents.push({ method: 'GET', url: '/api/command/', request: null, keystring: 'API_GET_COMMAND' });
    contents.push({ method: 'GET', url: '/api/filelist/', request: null, keystring: 'API_GET_FILELIST' });
    MultiApiCall(contents, displayCommand);
}

function displayCommand() {
    drowContainerFluid($('<div id="command" class="col-xs-12" style="display:block"></div>'));

    if (commonErrorHandling('#command', 'API_GET_COMMAND') != 0 || commonErrorHandling('#command', 'API_GET_FILELIST') != 0) {
        return;
    }  

    let commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    if (commandList == null) {
        $('#command').append(getClientMessage('NOCMDEXIST'));
    }

    switchCommandButton();

    if (responseData['API_GET_COMMAND'].Data.Command !== undefined) {
        let commandTableDiv = $('<div id="commandtable" class="table-responsive">');
        let commandListTable = $('<table>');
        commandListTable.addClass('table stktable table-striped');

        let tmpChkBoxClm = '';
        if (userRole != 1) {
            tmpChkBoxClm = '<th></th>';
        }
    
        let tHead = $('<thead class="thead-light">');
        tHead.append('<tr>' + tmpChkBoxClm +
                    '<th>' + getClientMessage('COMNAME') + '</th>' +
                    '<th>' + getClientMessage('COMTYPE') + '</th>' +
                    '<th class="d-none d-lg-table-cell">' + getClientMessage('COMFILESTOAGT') + '</th>' +
                    '<th class="d-none d-lg-table-cell">' + getClientMessage('COMSCRIPTDEF') + '</th>' +
                    '<th class="d-none d-lg-table-cell">' + getClientMessage('COMFILESTOSVR') + '</th></tr>');
        commandListTable.append(tHead);

        let tBody = $('<tbody>');
        for (let Loop = 0; Loop < commandList.length; Loop++) {
            let tmpChkBoxStr = '';
            if (userRole != 1) {
                tmpChkBoxStr = '<td><div class="checkbox"><input type="checkbox" id="cmdId' + commandList[Loop].Id + '" value="" onclick="switchCommandButton()"/></div></td>';
            }
    
            let typeStr = '';
            if (commandList[Loop].Type == 0) {
                typeStr = 'Linux /usr/bin/bash';
            } else {
                typeStr = 'Windows cmd.exe /c';
            }

            let filesToAgt = '';
            for (let loopSvr = 0; loopSvr < commandList[Loop].ServerFileName.length; loopSvr++) {
                if (commandList[Loop].ServerFileName[loopSvr] !== '') {
                    filesToAgt = '<div align="center"><span class="icon icon-checkmark" style="font-size:18px;"></span></div>';
                    break;
                }
            }

            let filesToSvr = '';
            for (let loopAgt = 0; loopAgt < commandList[Loop].AgentFileName.length; loopAgt++) {
                if (commandList[Loop].AgentFileName[loopAgt] !== '') {
                    filesToSvr = '<div align="center"><span class="icon icon-checkmark" style="font-size:18px;"></span></div>';
                    break;
                }
            }

            let scriptDef = '';
            if (commandList[Loop].Script != null && commandList[Loop].Script !== '') {
                scriptDef = '<div align="center"><span class="icon icon-checkmark" style="font-size:18px;"></span></div>';
            }

            tBody.append('<tr>' + tmpChkBoxStr +
                        '<td><a id="cmdprop' + commandList[Loop].Id + '" style="cursor: pointer;"><u>' + commandList[Loop].Name + '</u></a></td>' +
                        '<td>' + typeStr + '</td>' +
                        '<td class="d-none d-lg-table-cell">' + filesToAgt + '</td>' +
                        '<td class="d-none d-lg-table-cell">' + scriptDef + '</td>' +
                        '<td class="d-none d-lg-table-cell">' + filesToSvr + '</td></tr>');
        }
        commandListTable.append(tBody);
        commandTableDiv.append(commandListTable);
        $('#command').append(commandTableDiv);
        for (let loopclick = 0; loopclick < commandList.length; loopclick++) {
            $('#cmdprop' + commandList[loopclick].Id).on('click', function() {
                displayCommandSetting(commandList[loopclick].Id);
            });
        }
    }
    resizeComponent();
}

function switchCommandButton() {
    let commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
    let cnt = 0;
    for (let loop = 0; commandList != null && loop < commandList.length; loop++) {
        if ($('#cmdId' + commandList[loop].Id).prop('checked') == true) {
            cnt++;
        }
    }
    clearRsCommand();
    if (userRole != 1) {
        if (cnt > 0) {
            addRsCommand("displayCommandSetting(-1)", "icon-plus", true);
            addRsCommand("displayDeleteCommandDlg()", "icon-bin", true);
        } else {
            addRsCommand("displayCommandSetting(-1)", "icon-plus", true);
            addRsCommand("", "icon-bin", false);
        }
    } else {
        addRsCommand("", "icon-plus", false);
        addRsCommand("", "icon-bin", false);
    }
}

{
    let serverFileNameCount = 0;
    let agentFileNameCount = 0;
    let serverFileNameIndex = 0;
    let agentFileNameIndex = 0;

    function initServerFileNameCount() {
        serverFileNameCount = 0;
        serverFileNameIndex = 0;
    }

    function initAgentFileNameCount() {
        agentFileNameCount = 0;
        agentFileNameIndex = 0;
    }

    function addServerFileName() {
        if (serverFileNameCount >= 5) {
            return;
        }
        $('#serverFileName-inputgroup').append(
          '<div id="serverFileName-inputgroup-child' + serverFileNameIndex + '" class="input-group">' +
            '<select id="serverFileNameS' + serverFileNameIndex + '" class="form-control">' +
              '<option></option>' +
            '</select>' +
            '<div class="input-group-append">' +
              '<button class="btn btn-outline-secondary" type="button" onclick="addServerFileName()"><span class="icon icon-plus" style="font-size:18px;"></span></button>' +
              '<button class="btn btn-outline-secondary" type="button" onclick="removeServerFileName(' + serverFileNameIndex + ')"><span class="icon icon-bin" style="font-size:18px;"></span></button>' +
            '</div>' +
          '</div>'
        );
        if (responseData['API_GET_FILELIST'].Data !== undefined) {
            let fileList = getArray(responseData['API_GET_FILELIST'].Data.FileInfo);
            for (let loop = 0; loop < fileList.length; loop++) {
                $('#serverFileNameS' + serverFileNameIndex).append('<option>' + fileList[loop].Name + '</option>');
            }
        }
        serverFileNameCount++;
        serverFileNameIndex++;
    }

    function removeServerFileName(target) {
        if (serverFileNameCount <= 1) {
            return;
        }
        $('#serverFileName-inputgroup-child' + target).remove();
        serverFileNameCount--;
    }

    function addAgentFileName() {
        if (agentFileNameCount >= 5) {
            return;
        }
        $('#agentFileName-inputgroup').append(
          '<div id="agentFileName-inputgroup-child' + agentFileNameIndex + '" class="input-group">' +
            '<input id="agentFileNameS' + agentFileNameIndex + '" class="form-control" type="text" placeholder="' + getClientMessage('COMPLACEAGT') + '"/>' +
            '<div class="input-group-append">' +
              '<button class="btn btn-outline-secondary" type="button" onclick="addAgentFileName()"><span class="icon icon-plus" style="font-size:18px;"></span></button>' +
              '<button class="btn btn-outline-secondary" type="button" onclick="removeAgentFileName(' + agentFileNameIndex + ')"><span class="icon icon-bin" style="font-size:18px;"></span></button>' +
            '</div>' +
          '</div>'
        );
        agentFileNameCount++;
        agentFileNameIndex++;
    }

    function removeAgentFileName(target) {
        if (agentFileNameCount <= 1) {
            return;
        }
        $('#agentFileName-inputgroup-child' + target).remove();
        agentFileNameCount--;
    }

    function displayCommandSetting(targetId) {
        let commandSettingDlg = $('<div/>')
        commandSettingDlg.append('<div class="form-group"><label for="commandName">' + getClientMessage('COMNAME') + '</label><input type="text" class="form-control" id="commandName" placeholder="' + getClientMessage('COMNAME') + '"></div>');
        commandSettingDlg.append('<label for="serverFileName">' + getClientMessage('COMCOPYTOAGT') + '</label><div id="serverFileName-inputgroup"/>');
        commandSettingDlg.append('<p></p>');
        commandSettingDlg.append('<div class="form-group"><label for="commandType">' + getClientMessage('COMTYPE') + '</label><select class="form-control" id="commandType"><option>Linux /usr/bin/bash</option><option>Windows cmd.exe /c</option></select></div>');
        commandSettingDlg.append('<div class="form-group"><label for="commandScript">' + getClientMessage('COMSCRIPT') + '<br/>' + getClientMessage('COMMANDEXITCODE') + '</label><textarea class="form-control" id="commandScript" rows="3" style="margin-top: 0px; margin-bottom: 0px; height: 185px;"></textarea></div>');
        commandSettingDlg.append('<div class="form-group"><label for="timeout">' + getClientMessage('COMTIMEOUT') + '</label><input type="text" class="form-control" id="timeout"/></div>');
        commandSettingDlg.append('<label for="agentFileName">' + getClientMessage('COMCOPYTOSVR') + '</label><div id="agentFileName-inputgroup"/>');
        commandSettingDlg.append('<p></p>');
        commandSettingDlg.append('<div id="command_errmsg"/>');
        commandSettingDlg.append('<p></p>');
    
        if (userRole != 1) {
            if (targetId == -1) {
                commandSettingDlg.append('<button type="button" id="commandBtnAdd" class="btn btn-dark" onclick="updateCommand(false,' + targetId + ')">' + getClientMessage('COMADD') + '</button> ');
            } else {
                commandSettingDlg.append('<button type="button" id="commandBtnUpdate" class="btn btn-dark" onclick="updateCommand(true, ' + targetId + ')">' + getClientMessage('COMUPDATE') + '</button> ');
            }
            commandSettingDlg.append('<button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('COMCANCEL') + '</button> ');
        } else {
            commandSettingDlg.append('<button type="button" class="btn btn-dark" onclick="closeInputModal()">' + getClientMessage('COMCLOSE') + '</button> ');
        }
    
        let titleStr = '';
        if (targetId == -1) {
            titleStr = getClientMessage('COMADDCOMMAND');
        } else {
            titleStr = getClientMessage('COMEDITCOMMAND');
        }
        showInputModal('<h5 class="modal-title">' + titleStr + '</h5>', commandSettingDlg);

        $('#timeout').val('60');

        initServerFileNameCount();
        initAgentFileNameCount();
        addServerFileName();
        addAgentFileName();
    
        var commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        if (commandList == null) {
            return;
        }
        for (loop = 0; loop < commandList.length; loop++) {
            if (commandList[loop].Id == targetId) {
                var typeStr = '';
                if (commandList[loop].Type == 0) {
                    typeStr = 'Linux /usr/bin/bash';
                } else {
                    typeStr = 'Windows cmd.exe /c';
                }
                $('#commandName').val(commandList[loop].Name);
                for (let loopSvr = 0; loopSvr < commandList[loop].ServerFileName.length; loopSvr++) {
                    if (commandList[loop].ServerFileName[loopSvr] !== '') {
                        if (loopSvr >= 1) {
                            addServerFileName();
                        }
                        $('#serverFileNameS' + loopSvr).val(commandList[loop].ServerFileName[loopSvr]);
                    }
                }
                $('#commandType').val(typeStr);
                $('#commandScript').val(commandList[loop].Script);
                $('#timeout').val(commandList[loop].Timeout);
                for (let loopAgt = 0; loopAgt < commandList[loop].AgentFileName.length; loopAgt++) {
                    if (commandList[loop].AgentFileName[loopAgt] !== '') {
                        if (loopAgt >= 1) {
                            addAgentFileName();
                        }
                        $('#agentFileNameS' + loopAgt).val(commandList[loop].AgentFileName[loopAgt]);
                    }
                }
            }
        }
    }

    function displayDeleteCommandDlg() {
        let commandInfo = getArray(responseData['API_GET_COMMAND'].Data.Command);
        let foundFlag = false;
        for (var loop = 0; commandInfo != null && loop < commandInfo.length; loop++) {
            if ($('#cmdId' + commandInfo[loop].Id).prop('checked') == true) {
                foundFlag = true;
            }
        }
        if (foundFlag == false) {
            return;
        }
    
        let deleteCommandDlg = $('<div/>')
        deleteCommandDlg.append(getClientMessage('COMDELCONFIRM'));
        deleteCommandDlg.append('<p></p>');
        for (let loop = 0; commandInfo != null && loop < commandInfo.length; loop++) {
            if ($('#cmdId' + commandInfo[loop].Id).prop('checked') == true) {
                deleteCommandDlg.append('&nbsp;&nbsp;&nbsp;' + commandInfo[loop].Name + '<br/>');
            }
        }
        deleteCommandDlg.append('<p></p>');
        deleteCommandDlg.append('<div id="command_errmsg"/>');
        deleteCommandDlg.append('<p></p>');
    
        deleteCommandDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="deleteCommand()">Delete</button> ');
        deleteCommandDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">Cancel</button> ');
    
        showInputModal('<h5 class="modal-title">' + getClientMessage('COMDELCOMMAND') + '</h5>', deleteCommandDlg);
    }

    function updateCommand(updateFlag, targetId) {
        if (updateFlag == true && targetId == -1) {
            return;
        }
        let comType = $("#commandType").val();
        let comTypeInt = -1;
        if (comType === 'Linux /usr/bin/bash') {
            comTypeInt = 0;
        } else if (comType === 'Windows cmd.exe /c') {
            comTypeInt = 1;
        }
        let timeout = parseInt($("#timeout").val());
        if (timeout == null || isNaN(timeout)) {
            timeout = 0;
        }

        // Padding of file array : BEGIN
        let serverFileNameAry = ['', '', '', '', ''];
        let agentFileNameAry = ['', '', '', '', ''];
        let svrFileCnt = 0;
        let agtFileCnt = 0;
        let tmpServerFileNameAry = $('[id^="serverFileNameS"]');
        let tmpAgentFileNameAry = $('[id^="agentFileNameS"]');
        for (let loop = 0; loop < tmpServerFileNameAry.length; loop++) {
            if (tmpServerFileNameAry[loop].value !== '') {
                serverFileNameAry[svrFileCnt] = tmpServerFileNameAry[loop].value;
                svrFileCnt++;
            }
        }
        for (let loop = 0; loop < tmpAgentFileNameAry.length; loop++) {
            if (tmpAgentFileNameAry[loop].value !== '') {
                agentFileNameAry[agtFileCnt] = tmpAgentFileNameAry[loop].value;
                agtFileCnt++;
            }
        }
        // Padding of file array : END

        if (updateFlag == false) {
            var ReqObj = {
                Name : $("#commandName").val(),
                Type : comTypeInt,
                Script : $("#commandScript").val(),
                ServerFileName : serverFileNameAry,
                AgentFileName : agentFileNameAry,
                Timeout : timeout
            };
            apiCall('POST', '/api/command/', ReqObj, 'API_POST_COMMAND', refreshAfterUpdateCommand);
        } else {
            var ReqObj = {
                Id : targetId,
                Name : $("#commandName").val(),
                Type : comTypeInt,
                Script : $("#commandScript").val(),
                ServerFileName : serverFileNameAry,
                AgentFileName : agentFileNameAry,
                Timeout : timeout
            };
            apiCall('POST', '/api/command/', ReqObj, 'API_POST_COMMAND', refreshAfterUpdateCommand);
        }
    }
    
    function deleteCommand() {
        let contents = [];
        let commandList = getArray(responseData['API_GET_COMMAND'].Data.Command);
        for (let loop = 0; commandList != null && loop < commandList.length; loop++) {
            if ($('#cmdId' + commandList[loop].Id).prop('checked') == true) {
                contents.push({ method: 'DELETE', url: '/api/command/' + commandList[loop].Id + '/', request: null, keystring: 'API_DELETE_COMMAND' });
            }
        }
        initSequentialApiCall();
        sequentialApiCall(contents, refreshAfterDeleteCommand);
    }
    
    function refreshAfterUpdateCommand() {
        if (commonErrorHandling('#command_errmsg', 'API_POST_COMMAND') != 0) {
            return;
        }
        closeInputModal();
        transDisplayCommand();
    }
    
    function refreshAfterDeleteCommand() {
        finalSequentialApiCall();
        if (commonErrorHandling('#command_errmsg', 'API_DELETE_COMMAND') != 0) {
            return;
        }
        closeInputModal();
        transDisplayCommand();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

{
    let targetId = -1;

    function transDisplayCommandResult() {
        apiCall('GET', '/api/commandresult/', null, 'API_GET_COMMANDRESULT', displayCommandResult);
    }
    
    function displayCommandResult() {
        drowContainerFluid($('<div id="commandresult" class="col-xs-12" style="display:block"></div>'));
        clearRsCommand();
    
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
                         '<th>' + getClientMessage('RESULT_UPDTIME') + '</th>' + '<th>' + getClientMessage('RESULT_AGTNAME') + ' / ' + getClientMessage('RESULT_CMDNAME') + '</th>' + '<th>' + getClientMessage('RESULT_RESULT') + '</th>' +
                         '</tr>');
            tableListData.append(tHead);
    
            let tBody = $('<tbody>');
            for (let Loop = 0; Loop < commandresultList.length; Loop++) {
                let dateUpdTime = getDateAndTimeStr(commandresultList[Loop].UpdTime);
                let resultId = commandresultList[Loop].Id;
                let tmpStyle = '';
                if (commandresultList[Loop].Status == 0) {
                    tmpStyle = 'background-color:LightGreen;';
                } else if (commandresultList[Loop].Status <= -980 && commandresultList[Loop].Status >= -989) {
                    tmpStyle = 'background-color:LightSkyBlue;';
                } else {
                    tmpStyle = 'background-color:LightCoral;';
                }
        
                tBody.append('<tr><td>' + dateUpdTime + '</td>' +
                            '<td>' + commandresultList[Loop].AgentName + ' / ' + commandresultList[Loop].CommandName + '</td>' +
                            '<td><div style="float:left;"><a id="resultAncId' + resultId + '" style="cursor: pointer;">' + '<span class="icon icon-terminal" style="font-size:26px;"></span></a></div><div align="center" style="' + tmpStyle + 'float:left;">' + getStatusLabel(commandresultList[Loop].Status) + '</div></td></tr>');
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
        targetId = resultId;
        apiCall('GET', '/api/commandresult/' + resultId + '/', null, 'API_GET_OUTPUT', viewConsole);
    }
    
    function viewConsole() {
        let consoleDlg = $('<div id="viewconsole"/>');
        showInputModal('<h5 class="modal-title">' + getClientMessage('RESULT_TITLE') + '</h5>', consoleDlg);
    
        if (statusCode['API_GET_OUTPUT'] == -1 || statusCode['API_GET_OUTPUT'] == 0) {
            displayAlertDanger('#viewconsole', getClientMessage('CONNERR'));
        } else if (statusCode['API_GET_OUTPUT'] != 200) {
            displayAlertDanger('#viewconsole', getSvrMsg(responseData['API_GET_OUTPUT']));
        } else {
            let commandresultList = getArray(responseData['API_GET_COMMANDRESULT'].Data.Result);
            for (let Loop = 0; Loop < commandresultList.length; Loop++) {
                if (commandresultList[Loop].Id == targetId) {
                    if (commandresultList[Loop].Status == 0) {
                        tmpStyle = 'background-color:LightGreen;';
                    } else if (commandresultList[Loop].Status <= -980 && commandresultList[Loop].Status >= -989) {
                        tmpStyle = 'background-color:LightSkyBlue;';
                    } else {
                        tmpStyle = 'background-color:LightCoral;';
                    }
                    consoleDlg.append(getClientMessage('RESULT_UPDTIME') + ' : ' + getDateAndTimeStr(commandresultList[Loop].UpdTime) + '<br/>');
                    consoleDlg.append(getClientMessage('RESULT_AGTNAME') + ' : ' + commandresultList[Loop].AgentName + '<br/>');
                    consoleDlg.append(getClientMessage('RESULT_CMDNAME') + ' : ' + commandresultList[Loop].CommandName + '<br/>');
                    consoleDlg.append(getClientMessage('RESULT_RESULT') + ' : ' + '<div style="' + tmpStyle + '">' + getStatusDetailLabel(commandresultList[Loop].Status) + '</div>');

                    let commandOutput = responseData['API_GET_OUTPUT'].Data.Result;
                    let consoleArea = $('<div style="overflow-wrap: break-word; padding: 6px 6px 10px 10px; color: #ffffff; background-color: #000000; font-family: monospace;"/>');
                    consoleArea.html(commandOutput.Output);
                    consoleDlg.append(consoleArea);
                    if (commandresultList[Loop].Status == 0 ||
                        commandresultList[Loop].Status == 1 ||
                        commandresultList[Loop].Status == -991) {
                            consoleDlg.append(getClientMessage('RESULT_EXITCODE') + ' : ' + commandresultList[Loop].ExitCode + '<br/>');
                        }
                }
            }
        }
        consoleDlg.append('<p/>')
        consoleDlg.append('<button type="button" class="btn btn-dark" onclick="closeInputModal();">' + getClientMessage('RESULT_CONSOLECLOSE') + '</button> ');
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

{
    // Name of selected time series data
    let selectedTsd = new Array('', '', '', '', '', '', '', '');

    // Current time for drawing agent status history
    let curTime = 0;

    function transShowSelectTimeSeriesDataDlg() {
        apiCall('GET', '/api/viewsetting/', null, 'API_GET_VIEWSETTING', showSelectTimeSeriesDataDlg);
    }

    function showSelectTimeSeriesDataDlg() {
        let selectTsdDlg = $('<div/>');
        showInputModal('<h5 class="modal-title">' + getClientMessage('DASHBOARD_SELECt_AGENT') + '</h5>', selectTsdDlg);
    
        if (statusCode['API_GET_VIEWSETTING'] == -1 || statusCode['API_GET_VIEWSETTING'] == 0) {
            selectTsdDlg.append('<div id="stsError"/>');
            displayAlertDanger('#stsError', getClientMessage('CONNERR'));
            return;
        }
        if (statusCode['API_GET_VIEWSETTING'] != 200) {
            selectTsdDlg.append('<div id="stsError"/>');
            displayAlertDanger('#stsError', getSvrMsg(responseData['API_GET_VIEWSETTING']));
            return;
        }

        selectTsdDlg.append(getClientMessage('DASHBOARD_STATUS_DESC'));

        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        let viewSettings = getArray(responseData['API_GET_VIEWSETTING'].Data.ViewSetting);
        for (let loop = 0; loop < 8; loop++) {
            selectedTsd[loop] = viewSettings[loop];
        }
        for (let loop = 0; loop < 8; loop++) {
            let curTsd = selectedTsd[loop];
            if (selectedTsd[loop] === '') {
                curTsd = getClientMessage('DASHBOARD_UNSPECIFIED');
            }
            let btnGrp = $('<div class="btn-group" style="width:90%;">');
            btnGrp.append('<button type="button" class="btn btn-dark dropdown-toggle" data-toggle="dropdown" aria-expanded="false"><span id="selecteTsd' + loop + '">' + curTsd + '</span><span class="caret"></span></button>');
            let ddMenu = $('<ul class="dropdown-menu" role="menu" style="width:100%;">');
            ddMenu.append('<li role="presentation"><a onclick="selectTimeSeriesData(' + loop + ', -1)" role="menuitem" tabindex="-1" href="#">' + getClientMessage('DASHBOARD_UNSPECIFIED') + '</a></li>');
            for (let loopMenu = 0; loopMenu < agentInfos.length; loopMenu++) {
                ddMenu.append('<li role="presentation"><a onclick="selectTimeSeriesData(' + loop + ', ' + loopMenu + ')" role="menuitem" tabindex="-1" href="#">' + agentInfos[loopMenu].Name + '</a></li>');
            }
            btnGrp.append(ddMenu);
        
            selectTsdDlg.append('<p></p>');
            selectTsdDlg.append('' + (loop + 1) + ': ');
            selectTsdDlg.append(btnGrp);
            selectTsdDlg.append('<p></p>');
        }
        selectTsdDlg.append('<div id="stsError"/>');
    
        selectTsdDlg.append('<p></p>');
        selectTsdDlg.append('<button type="button" id="OK" class="btn btn-dark" onclick="changeTimeSeriesData()">OK</button> ');
        selectTsdDlg.append('<button type="button" id="Cancel" class="btn btn-dark" onclick="closeInputModal()">Cancel</button> ');
        selectTsdDlg.append('<p></p>');
    }
    
    function selectTimeSeriesData(index, name) {
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (name == -1) {
            selectedTsd[index] = "";
            $('#selecteTsd' + index).text(getClientMessage('DASHBOARD_UNSPECIFIED'));
        } else {
            selectedTsd[index] = agentInfos[name].Name;
            $('#selecteTsd' + index).text(selectedTsd[index]);
        }
    }

    function changeTimeSeriesData() {
        var ReqObj = { "ViewSetting" : [
            {"Index" : 1, "TargetAgent" : selectedTsd[0]},
            {"Index" : 2, "TargetAgent" : selectedTsd[1]},
            {"Index" : 3, "TargetAgent" : selectedTsd[2]},
            {"Index" : 4, "TargetAgent" : selectedTsd[3]},
            {"Index" : 5, "TargetAgent" : selectedTsd[4]},
            {"Index" : 6, "TargetAgent" : selectedTsd[5]},
            {"Index" : 7, "TargetAgent" : selectedTsd[6]},
            {"Index" : 8, "TargetAgent" : selectedTsd[7]},
        ] };
        apiCall('POST', '/api/viewsetting/', ReqObj, 'API_POST_VIEWSETTING', afterChangeTimeSeriesData);
    }

    function afterChangeTimeSeriesData() {
        if (statusCode['API_POST_VIEWSETTING'] == -1 || statusCode['API_POST_VIEWSETTING'] == 0) {
            displayAlertDanger('#stsError', getClientMessage('CONNERR'));
            return;
        }
        if (statusCode['API_POST_VIEWSETTING'] != 200) {
            displayAlertDanger('#stsError', getSvrMsg(responseData['API_POST_VIEWSETTING']));
            return;
        }
        closeInputModal();
        transDisplayDashboard();
    }

    function transDisplayDashboard() {
        let contents = [];
        contents.push({ method: 'GET', url: '/api/agent/', request: null, keystring: 'API_GET_AGTINFO' });
        contents.push({ method: 'GET', url: '/api/viewsetting/', request: null, keystring: 'API_GET_VIEWSETTING' });
        MultiApiCall(contents, displayDashboard);
    }
    
    function displayDashboard() {
        drowContainerFluid($('<div id="dashboard" class="col-xs-12" style="display:block"></div>'));
        clearRsCommand();

        if (statusCode['API_GET_AGTINFO'] == -1 || statusCode['API_GET_AGTINFO'] == 0 ||
            statusCode['API_GET_VIEWSETTING'] == -1 || statusCode['API_GET_VIEWSETTING'] == 0) {
            displayAlertDanger('#dashboard', getClientMessage('CONNERR'));
            return;
        }
        if (statusCode['API_GET_AGTINFO'] != 200) {
            displayAlertDanger('#dashboard', getSvrMsg(responseData['API_GET_AGTINFO']));
            return;
        }
        if (statusCode['API_GET_VIEWSETTING'] != 200) {
            displayAlertDanger('#dashboard', getSvrMsg(responseData['API_GET_VIEWSETTING']));
            return;
        }
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (agentInfos == null) {
            $('#dashboard').append(getClientMessage('NOAGTINFO') + '<br/>');
            $('#dashboard').append(getClientMessage('DASHBOARD_STEP') + '<br/>');
            $('#dashboard').append('<ol>'
                + '<li>' + getClientMessage('DASHBOARD_STEP1') +  '</li>'
                + '<li>' + getClientMessage('DASHBOARD_STEP2') +  '</li>'
                + '<li>' + getClientMessage('DASHBOARD_STEP3') +  '</li>'
                + '</ol>');
            return;
        }
        $('#dashboard').append('<h5>&nbsp;&nbsp;&nbsp;&nbsp;' + getClientMessage('DASHBOARD_CURRENTSTATUS') + '&nbsp;&nbsp;</h5>');
        drasPieChart($('#dashboard'), agentInfos);
    
        let selectAgentBtn = '';
        if (userRole != 1) {
            selectAgentBtn = '<a onclick="transShowSelectTimeSeriesDataDlg();" class="plane-link" style="cursor: pointer;" href="#"><span class="icon icon-cog" style="font-size:20px;"></span></a>';
        }
        $('#dashboard').append('<h5><div class="plane-link">&nbsp;&nbsp;&nbsp;&nbsp;' + getClientMessage('DASHBOARD_48HSTATUS') + '&nbsp;&nbsp;' + selectAgentBtn + '</div></h5>');
        let viewSettings = getArray(responseData['API_GET_VIEWSETTING'].Data.ViewSetting);
        for (let loop = 0; loop < 8; loop++) {
            selectedTsd[loop] = viewSettings[loop];
        }
    
        let contents = [];
        for (let loop = 0; loop < 8; loop++) {
            if (selectedTsd[loop] === getClientMessage('DASHBOARD_UNSPECIFIED') || selectedTsd[loop] === '') {
                continue;
            }
            let tmpUrl = '/api/timeseriesdata/' + selectedTsd[loop] + '/';
            contents.push({ method: 'GET', url: tmpUrl, request: null, keystring: 'API_GET_TIMESERIESDATA_' + loop });
        }
        if (contents.length != 0) {
            initSequentialApiCall();
            sequentialApiCall(contents, transDrawAgentStatusHitory);
        }
    }

    function resetCurDate() {
        let curDate = new Date();
        let curTimeInMs = curDate.getTime();
        curTime = Math.floor(curTimeInMs / 1000);
    }

    function transDrawAgentStatusHitory() {
        finalSequentialApiCall();
        resetCurDate();
        drawAgentStatusHistory();
    }

    function drawAgentStatusHistory() {
        let escapeSelectorString = function(val){
            return val.replace(/[ !"#$%&'()*+,.\/:;<=>?@\[\\\]^`{|}~]/g, "\\$&");
        }

        for (let loop = 0; loop < 8; loop++) {
            if (selectedTsd[loop] === getClientMessage('DASHBOARD_UNSPECIFIED') || selectedTsd[loop] === '') {
                continue;
            }
            let timeseriesdata = getArray(responseData['API_GET_TIMESERIESDATA_' + loop].Data.TimeSeriesData);

            let newSvg = drawAgentStatusHistoryImpl(selectedTsd[loop], timeseriesdata, window.innerWidth);

            if ($('#' + escapeSelectorString(selectedTsd[loop])).length) {
                $('#' + escapeSelectorString(selectedTsd[loop])).replaceWith(newSvg);
            } else {
                $('#dashboard').append(newSvg);
            }
        }
    }
    
    function drawAgentStatusHistoryImpl(agentName, timeseriesdata, wsize) {
        let hsize = 70;
    
        let startTime = curTime - 172800; // 172800 = 60sec * 60min * 24hour * 2days
        let odbTime = curTime - 86400; // 86400 = 60sec * 60min * 24hour
        let unitw = (wsize - 90) / 172800;
        let rectStr = '';
        for (let loopTsd = 0; timeseriesdata != null && loopTsd < timeseriesdata.length; loopTsd++) {
            let theColor = '';
            if (timeseriesdata[loopTsd].Status == 0) {
                theColor = 'LightGreen';
            } else if (timeseriesdata[loopTsd].Status <= -980 && timeseriesdata[loopTsd].Status >= -989) {
                theColor = 'LightSkyBlue';
            } else {
                theColor = 'LightCoral';
            }
        
            let updTimeInt = parseInt(timeseriesdata[loopTsd].UpdTime, 16);
            if (updTimeInt < startTime) {
                continue;
            }
            let graphX = unitw * (updTimeInt - startTime) + 50;
            let graphWidth = unitw * timeseriesdata[loopTsd].SaInterval + 1;
            if (graphX + graphWidth > wsize - 40) {
                graphWidth = wsize - 40 - graphX;
            }
            let dateUpdTime = getDateAndTimeStr(timeseriesdata[loopTsd].UpdTime);
            let label = dateUpdTime + ':' + getStatusLabel(timeseriesdata[loopTsd].Status);
            rectStr = rectStr + '<rect x="' + graphX + '" y="20" width="' + graphWidth + '" height="30" fill="' + theColor + '"><title>' + label + '</title></rect>';
        }
        let startTimeDate = new Date(startTime * 1000);
        let odbTimeDate = new Date(odbTime * 1000);
    
        let newSvg = $(
            '<svg id="' + agentName + '" xmlns="http://www.w3.org/2000/svg" width="' + (wsize - 40) + '" height="' + hsize + '" viewBox="0 0 ' + (wsize - 40) + ' ' + hsize + '">' +
            '<text x="10" y="19" fill="black">' + agentName + '</text>' +
            '<rect x="50" y="20" width="' + (wsize - 90) + '" height="30" fill="Silver"></rect>' +
            rectStr +
            '<line x1="50" y1="40" x2="50" y2="65" stroke="blue" stroke-width="2"/>' +
            '<text x="53" y="65" fill="blue">' + (startTimeDate.getMonth() + 1) + '/' + startTimeDate.getDate() + ' ' + ('00' + startTimeDate.getHours()).slice(-2) + ':' + ('00' + startTimeDate.getMinutes()).slice(-2) + '</text>' +
            '<line x1="' + ((wsize - 90) / 2 + 50) + '" y1="40" x2="' + ((wsize - 90) / 2 + 50) + '" y2="65" stroke="blue" stroke-width="2"/>' +
            '<text x="' + ((wsize - 90) / 2 + 53) + '" y="65" fill="blue">' + (odbTimeDate.getMonth() + 1) + '/' + odbTimeDate.getDate() + ' ' + ('00' + odbTimeDate.getHours()).slice(-2) + ':' + ('00' + odbTimeDate.getMinutes()).slice(-2) + '</text>' +
            '</svg>'
        );
        return newSvg;
    }

    function drasPieChart(element, agentInfos) {
        let totalAgentCnt = agentInfos.length;
        let successCnt = 0;
        let eventCnt = 0;
        let errorCnt = 0;
        let noRequestCnt = 0;
        for (let loop = 0; loop < totalAgentCnt; loop++) {
            if (agentInfos[loop].Status <= -970 && agentInfos[loop].Status >= -979) {
                noRequestCnt++;
            } else if (agentInfos[loop].Status <= -980 && agentInfos[loop].Status >= -989) {
                eventCnt++;
            } else if (agentInfos[loop].Status == 0) {
                successCnt++;
            } else {
                errorCnt++;
            }
        }
    
        let eventCntOffset = (successCnt / totalAgentCnt) * 314.15;
        let errorCntOffset = eventCntOffset + (eventCnt / totalAgentCnt) * 314.15;
        let noRequestCntOffset = errorCntOffset + (errorCnt / totalAgentCnt) * 314.15;
        let wsize = $(window).width();
        let hsize = 220;
     
        let newSvg = $(
            '<svg id="piechart" xmlns="http://www.w3.org/2000/svg" width="' + (wsize - 10) + '" height="' + hsize + '" viewBox="0 0 400 220">' +
            '<text x="250" y="40" font-size="20" fill="black">Total: ' + totalAgentCnt + '</text>' +
            '<text x="250" y="70" font-size="20" fill="black">Success: ' + successCnt + '</text>' +
            '<text x="250" y="100" font-size="20" fill="black">Normal: ' + eventCnt + '</text>' +
            '<text x="250" y="130" font-size="20" fill="black">Error: ' + errorCnt + '</text>' +
            '<text x="250" y="160" font-size="20" fill="black">No request: ' + noRequestCnt + '</text>' +
            '<rect x="230" y="55" width="18" height="18" fill="LightGreen"></rect>' +
            '<rect x="230" y="85" width="18" height="18" fill="LightSkyBlue"></rect>' +
            '<rect x="230" y="115" width="18" height="18" fill="LightCoral"></rect>' +
            '<rect x="230" y="145" width="18" height="18" fill="Silver"></rect>' +
            '<circle r="50" cx="110" cy="110" fill="rgba(0,0,0,0)" stroke="LightGreen" stroke-width="100" stroke-dashoffset="0" stroke-dasharray="314.15"/>' + 
            '<circle r="50" cx="110" cy="110" fill="rgba(0,0,0,0)" stroke="LightSkyBlue" stroke-width="100" stroke-dashoffset="' + eventCntOffset + '" stroke-dasharray="314.15"/>' +
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
                { actApiName: 'transDisplayLogInfo()', title: getClientMessage('EVENT_LOG') },
                { actApiName: 'transDisplayChgPassword()', title: getClientMessage('USER_CHG_PW') }
            ];
        } else {
            usermenuContents = [
                { actApiName: 'transDisplayLogInfo()', title: getClientMessage('EVENT_LOG') },
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
    let hsize_agentinfotable = window.innerHeight - 57;
    let hsize_filemgmttable = window.innerHeight - 57;
    let hsize_resulttable = window.innerHeight - 57;
    let hsize_commandtable = window.innerHeight - 57;
    $("#agentinfotable").css("height", hsize_agentinfotable + "px");
    $("#filemgmttable").css("height", hsize_filemgmttable + "px");
    $("#resulttable").css("height", hsize_resulttable + "px");
    $("#commandtable").css("height", hsize_commandtable + "px");

    // Dashboard resize
    if ($('#dashboard').length) {
        let agentInfos = getArray(responseData['API_GET_AGTINFO'].Data.AgentInfo);
        if (agentInfos != null) {
            drasPieChart($('#dashboard'), agentInfos);
            drawAgentStatusHistory();
        }
    }
    // Agent detail dialog resize
    if ($('#agentdetail').length) {
        let tmpWSize = 0;
        if (wsize >= 1200) {
            tmpWSize = 1000;
        } else if (wsize >= 992) {
            tmpWSize = 792;
        } else if (wsize >= 576) {
            tmpWSize = 476;
        } else if (wsize >= 400) {
            tmpWSize = 380;
        } else {
            tmpWSize = wsize - 20;
        }
        resizeAgentStatusHistory(tmpWSize);
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

# AWS deployment

# How to deploy Serval to AWS
- Install Serval server
- Allow port number 8081 for EC2 security group.
- svrUrl change pf stkcommon.js (for cross domain access)
- sample.conf servicehost=<IP address>
- Restart Serval server
- Update Serval client
- stkcommon.js ; change svrhost="https://commander.red-squirrels.net"

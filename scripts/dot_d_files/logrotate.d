/var/log/falco/*.log
/var/log/maldet/*.log
/var/log/clamav/*.log
/var/log/vessel-optimizer.log {
    daily
    rotate 7
    compress
    delaycompress
    missingok
    notifempty
    create 0640 root adm
    sharedscripts
    postrotate
        /usr/bin/systemctl reload falco > /dev/null 2>&1 || true
    endscript
}

#!/bin/bash
:<<d2lminusminusdemocode
#!/bin/bash

mkfifo .botfile
chan=$1
if [ $2 != '' ] && [ -f $2 ] ; then
      key=`cat $2`
  fi

  tail -f .botfile | openssl s_client -connect irc.cat.pdx.edu:6697 | while true; do
    if [[ -z $started ]] ; then
            echo "USER d2lbot d2lbot d2lbot :d2lbot" >> .botfile
                echo "NICK d2lminusminus" >> .botfile
                    echo "JOIN #$chan $key" >> .botfile
                        started="yes"
                          fi
                            read irc
                              echo $irc
                                if `echo $irc | cut -d ' ' -f 1 | grep PING > /dev/null`; then
                                        echo "PONG" >> .botfile
                                          elif `echo $irc | grep PRIVMSG | grep d2l > /dev/null` ; then
                                                  nick="${irc%%!*}"; nick="${nick#:}"
                                                      if [[ $nick != 'd2lminusminus' ]] ; then
                                                                chan=`echo $irc | cut -d ' ' -f 3`
                                                                      echo "PRIVMSG $chan :d2l--" >> .botfile
                                                                          fi
                                                                            fi
                                                                        done
d2lminusminusdemocode

# create a fifo to queue output to server for rate-limiting
mkfifo .botfile

# initial channel to join here:
CHAN=#oracle

tail -f .botfile | openssl s_client -connect irc.cat.pdx.edu:6697 | while true; do
    if [[ -z $STARTED ]]; then
        echo "USER oracle oracle oracle :oracle" >> .botfile
        echo "NICK oracle" >> .botfile
        echo "JOIN $CHAN" >> .botfile
        STARTED="yes"
    fi
    read IRC
    if `echo $IRC | cut -d ' ' -f 1 | grep -q PING`; then
        echo "PONG" >> .botfile
    elif `echo $IRC | grep "PRIVMSG" | grep -q '!card'`; then
# http://magiccards.info/query?q=ambiguity&v=card&s=cname
        #CARDNAME=`echo $IRC | sed 's/^[^!]*!card //'`
        CARDNAME=`echo $IRC | sed 's/^[^!]*!card //' | tr '[:upper:]' '[:lower:]'`
        CARDNAME_CAP="`echo ${CARDNAME:0:1} | tr '[lower]' '[:upper:]'`${CARDNAME:1}"
        # MTGI_RAW=$(curl "http://magiccards.info/query?q=`echo $CARDNAME | sed 's/ /+/'`&v=card&s=cname")
        MTGI_RAW=$(curl -s "http://magiccards.info/query?q=`echo $CARDNAME | sed 's/ /+/g'`&v=card&s=cname")
        MTGI_RAW_SUB=$(echo $MTGI_RAW | sed "s:^.*>$CARDNAME_CAP</a>.<img::" | sed 's:</table>.*$::')


#!/bin/bash


# create a fifo to queue output to server for rate-limiting
mkfifo .botfile

echo foo

# initial channel to join here:
CHAN=#mtg

tail -f .botfile | openssl s_client -connect irc.cat.pdx.edu:6697 | while true; do
    if [[ -z $STARTED ]]; then
        echo "USER oracle oracle oracle :oracle" >> .botfile
        echo "NICK magicbot" >> .botfile
        echo "JOIN $CHAN" >> .botfile
        STARTED="yes"
    fi
    read IRC
    echo "$IRC"
    if `echo $IRC | cut -d ' ' -f 1 | grep -q "PING"`; then
        echo "PONG" >> .botfile
    elif `echo $IRC | grep "PRIVMSG" | grep -q '!card'`; then
        
        # Set the target channel or user to send the message to
        TARGET=`echo "$IRC" | cut -d ' ' -f 3`

        # Fetch card name from IRC query string and store capitalized and non-capitalized versions of it as variables.
        CARDNAME=`echo -n $IRC | sed 's/.*!card //' | tr '[:upper:]' '[:lower:]' | tr -dc '[:print:]'`
        CARDNAME_CAP="`echo -n "${CARDNAME^}" | sed 's: .:\U&:g' `"
        CARDNAME_CAP="`echo -n "$CARDNAME_CAP" | sed 's:Of:of:g' | sed 's:The:the:'`"
echo "$CARDNAME_CAP" | xxd

        # Fetch raw data from magiccards.info with curl, which we can then pare down with sed parsing to extract the target card info.
        MTGI_RAW=$(curl -s "http://magiccards.info/query?q=`echo -n $CARDNAME | sed 's: :+:g' | sed "s:':\%27:g"`&v=card&s=cname")
        # the following line, found in the line above, is a WIP. Conversion from single quotes to %27 strings in url matching not yet working.
echo $CARDNAME | sed 's: :+:g' | sed "s:':\%27:g"
echo "http://magiccards.info/query?q=`echo -n $CARDNAME | sed 's: :+:g' | sed "s:':\%27:g"`&v=card&s=cname"
#echo "$MTGI_RAW"
#echo "$MTGI_RAW" | grep "`echo $CARDNAME_CAP | tr -dc '[:print:]'`"


        # Chop off heads and tails and ensure we're only working with the html for the proper target card, to eliminate multiple matches and false positives.
        MTGI_RAW_SUB=$(echo $MTGI_RAW | sed "s:^.*>$CARDNAME_CAP\( ([^)]*)\)\?</a>.<img::" | sed 's:</table>.*$::')

echo "$MTGI_RAW_SUB"

        # Error checking - If we haven't captured our target card, print an error and return fail.
        if echo $MTGI_RAW_SUB | grep -q "$CARDNAME_CAP"; then
            echo "Got a match!"
            : # do nothing
        else
            # Add an error report here
            echo "No match :("
            echo "PRIVMSG $TARGET :The oracle has no time for your games." | tee .botfile
            continue
        fi
        
        # consider using fold to break every n characters to avoid truncated replies, then use sed to cat server talk after every newline before continuations.
        MTGI_CARD_METRICS=`echo $MTGI_RAW_SUB | sed 's:</p>.*$::' | sed 's:^.*<p>::'`
        MTGI_CARD_BODY=`echo $MTGI_RAW_SUB | sed 's:</b>.*$::' | sed 's:^.*<b>::' | sed 's:<br><br>:\n%:g'`

        # Squirt variable contents into the out box

        echo "PRIVMSG $TARGET :$MTGI_CARD_METRICS" | tee .botfile
        echo "PRIVMSG $TARGET :$MTGI_CARD_BODY" | sed "s/%/PRIVMSG $TARGET :/g" | tee .botfile

    fi
done

echo "All done!"
exit 0

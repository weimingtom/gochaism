//-- [ common functions ] ------------------------------------------------------

//-- [ common functions ] ------------------------------------------------------

//-- [ subroutine ] ------------------------------------------------------------

//-- [ entry point ] -----------------------------------------------------------

function event::onChannelText(prefix, channel, text)
{
    onTextPost(prefix, channel, text);
}

function event::onTalkText(prefix, targetNick, text)
{
    if (targetNick == myNick)
        onTextPost(prefix, prefix.nick, text);
    else
        onTextPost(prefix, targetNick, text);
}

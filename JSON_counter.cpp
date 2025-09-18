let cnt = context.get('cnt') ||0;
cnt++;
context.set('cnt',cnt);
msg.payload=cnt;
return msg;

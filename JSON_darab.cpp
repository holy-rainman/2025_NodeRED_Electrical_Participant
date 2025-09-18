let kira = context.get('kira')||0;
kira++;
let darab = 5*kira;
context.set('kira',kira);
msg.payload=darab;
return msg;

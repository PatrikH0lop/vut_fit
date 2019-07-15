cp -r ./* ~/WWW/IIS/
cp ./.htaccess ~/WWW/IIS/
chmod -R o=rX,g= ~/WWW
chmod -R o+r ~/WWW/*
chmod o+r ~/WWW/IIS/.htaccess 

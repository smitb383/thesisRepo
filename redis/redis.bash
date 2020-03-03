#starting redis server for the first time 
$ brew install redis

$ redis-server --protected-mode no [start redis]
$ ln -sfv /usr/local/opt/redis/*.plist ~/Library/LaunchAgents [Launch Redis on startup]

$ sudo nano /usr/local/etc/redis.conf [Location of Redis configuration file.]

Uninstall Redis and its files.
$ brew uninstall redis
$ rm ~/Library/LaunchAgents/homebrew.mxcl.redis.plist

#starting redis after already installed 
$ redis-server --protected-mode no

echo "Thanks for installing Gitty :)"
shards install --quiet --production
make build && make install
echo "All tasks complete. Gitty is now installed."

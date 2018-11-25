dirs=`find -maxdepth 1 -type d`
for dir in $dirs; do
	pushd $dir > /dev/null
	if [ ! -d .git ]; then
		echo -n "$dir "
		git pull
	else
		echo "WARNING $dir is not a git-module"
	fi
	popd > /dev/null
done

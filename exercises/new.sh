name=$1
if [ "$name" = "" ]; then
    read -p "new exercise name: " name
fi

if [ -d $name ]; then
    echo "directory \"$name\" exists"
    exit 1
fi

cp -r sample $name

echo "new folder \"$name\" created"
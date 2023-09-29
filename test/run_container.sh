#!/bin/bash

# set default image if another is not specified
if test -z "$os"; then
  image_os="ubuntu"
else
  image_os="$os"
fi

# set identifiers
dockerfile="Dockerfile.$image_os"
image_name=$image_os:smartcalc
container_name="dondarri.${image_os}-temp"

# build the image if it is not built
if ! docker image inspect "$image_name" >/dev/null 2>&1; then
  docker image build -t "$image_name" - < test/"$dockerfile"
fi
# remove the previous container if it exists
docker container rm -f "$container_name" 2> /dev/null

docker container run --interactive \
                     --tty \
                     --rm \
                     --volume "$PWD"/:/home/ \
                     --workdir /home \
                     --env TZ=Asia/Novosibirsk \
                     --name "$container_name" \
                     "$image_name"

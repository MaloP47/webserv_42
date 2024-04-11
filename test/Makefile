build:
		@docker build -t nginxtest .

exec:
		@docker exec -it nginxtest /bin/bash

run:
		@docker run --name nginxtest -p 8080:80 -it nginxtest

erase:
		@docker image prune -af
		@docker system prune -af


.PHONY: build exec test

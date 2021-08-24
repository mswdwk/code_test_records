pipeline {
    agent { label 'node2' }
    stages {
        stage('Build') {
            steps {
				echo 'build start'
				echo `date`
				echo 'build ok'
            }
        }
		stage('test') {
            steps {
				echo 'test start'
				echo `date`
            }
        }
    }
}

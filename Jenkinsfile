pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
				bat 'echo build start'
                bat 'date /T'
				bat 'dir'
				bat 'echo build ok'
            }
        }
		stage('test') {
            steps {
				bat 'echo test start'
                bat 'date /T'
				bat 'echo test ok'
            }
        }
    }
}
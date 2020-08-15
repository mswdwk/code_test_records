pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
				bat 'echo build start'
                bat 'date /T'
				bat 'dir'
				bat 'echo hello world'
            }
        }
		stage('test') {
            steps {
				bat 'echo test start'
                bat 'date /T'
				bat 'pwd'
				bat 'echo test ok'
            }
        }
    }
}
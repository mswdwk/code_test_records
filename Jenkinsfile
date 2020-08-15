pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
				bat 'echo build start'
                bat 'date /T'
				bat 'dir'
				bat 'pwd'
				bat 'echo hello world'
            }
        }
		stage('test') {
            steps {
                bat 'date /T'
				bat 'dir'
				bat 'pwd'
				bat 'echo test ok'
            }
        }
    }
}
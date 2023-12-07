pipeline {
    agent { 
	    docker { 
             image 'centos:latest' 
			 label 'linux-node1'
		} 	
	}

    stages {
        stage('Build') {
            steps {
				sh 'echo "build start"'
				sh 'echo `date`'
				sh 'echo "build ok"'
            }
        }
		stage('test') {
            steps {
				sh '''
					echo "test start"
					echo `date`
				'''
            }
        }
		stage('deploy') {
            steps {
				sh '''
					echo "deploy start"
					echo `date`
				'''
            }
        }
		post {
		post {
        always {
            echo 'This will always run'
        }
        success {
            echo 'This will run only if successful'
        }
        failure {
            echo 'This will run only if failed'
        }
        unstable {
            echo 'This will run only if the run was marked as unstable'
        }
        changed {
            echo 'This will run only if the state of the Pipeline has changed'
            echo 'For example, if the Pipeline was previously failing but is now successful'
        }
    }
    }
}
}

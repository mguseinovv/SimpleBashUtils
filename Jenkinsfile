pipeline {
  agent any 
  stages {
    stage('Checkout') {
      steps {
        checkout scm
      }
    }
    stage('CodeStyle') {
      steps {
        sh '''
          set -e
          echo "-------------CODESTYLE TEST STARTED------------"
          if ! clang-format -n cat/* grep/*; then
            echo "code style failed"
            exit 1
          fi
          echo "-------------CODESTYLE TEST STARTED------------"
        '''
      }
    }
    stage('Build') {
      steps {
        set -e
        sh 'echo "-------------BUILD STARTED------------"'
        sh 'cd cat && make'
        sh 'cd ../grep && make'
        sh 'echo "-------------BUILD ENDED------------"'
        sh '''
          if [[ ! -f cat/s21_cat || ! -f grep/s21_grep ]]; then
            echo "files does't exist"
            exit 1
          fi
        '''
        archiveArtifacts artifacts: 'cat/s21_cat, grep/s21_grep', fingerprint: true
      }
    }
    stage('Deploy_to_Server') {
      steps {
        sh '''
          echo "-------------DEPLOY STARTED------------"
          scp cat/s21_cat root@172.18.0.3:root/CICD/ || { echo "failed to deploy s21_cat"; exit 1; }
          scp grep/s21_grep root@172.18.0.3:root/CICD/ || { echo "failed to deploy s21_grep"; exit 1; }
          echo "-------------DEPLOY ENDED------------"
        '''  
      }
    }
    post {
      failure {
        echo "pipeline failed"
      }
      success {
        echo "sorry che-to ya zasolil"
      }
    }
  }
}

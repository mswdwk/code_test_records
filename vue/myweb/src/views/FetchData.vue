<!--
这个示例会通过 GitHub 的 API 获取最新的 Vue Core 提交信息并将其展示为列表。
你可以在两个分支之间切换。
-->

<script>
const API_URL = `https://api.github.com/repos/vuejs/core/commits?per_page=`
const API_URL_2 = `http://lostcalhost:8082/esindex/getall`

export default {
  data: () => ({
    branches: ['main', 'minor'],
    currentBranch: 'main',
    commits: [],
    per_page: 3,
    count:0,
    index_names:[]
  }),

  created() {
    // 在初始化的时候进行获取
    this.fetchData()
  },

  watch: {
    // 当 currentBranch 改变时重新获取
    currentBranch: 'fetchData',
    count: 'postData'
  },

  methods: {
    async fetchData() {
      const url = `${API_URL}${this.per_page}&sha=${this.currentBranch}`
      this.commits = await (await fetch(url)).json()
    },
    async postData() {
      const url = `${API_URL_2}`

      try {
          response = await fetch(url,{
          methods: 'POST',
          body : JSON.stringify({ id : 'id_1',names : ["name1","name 2 ","hello world"]}),　
          headers:{
            'Content-Type' : 'application/json'
          }
        });

        this.index_names =await response.json();

        console.log("index_names = "+this.index_names)
      }catch(error){
        console.error("error is "+error);
      }
    },
    truncate(v) {
      const newline = v.indexOf('\n')
      return newline > 0 ? v.slice(0, newline) : v
    },
    formatDate(v) {
      return v.replace(/T|Z/g, ' ')
    }
  }
}
</script>


<template>
    <div class="FetchData">
    <h1>This is an fetch data page</h1>
 
  <h1>Latest Vue Core Commits</h1>
  <template v-for="branch in branches">
    <input type="radio"
      :id="branch"
      :value="branch"
      name="branch"
      v-model="currentBranch">
    <label :for="branch">{{ branch }}</label>
  </template>
  
  <p>页大小: {{ per_page }}</p>

    <button @click="count++" id="mybution" value="my but" >
    Search Count is: {{ count }}
  </button>

<select v-model="per_page">
  <option disabled value="">Please select page size</option>
  <option>3</option>
  <option>5</option>
  <option>10</option>
</select>

  <p>vuejs/core@{{ currentBranch }}</p>
  <ul v-if="commits.length > 0" id="my_commit">
    <li v-for="({ html_url, sha, author, commit },index) in commits" :key="sha">
      {{ index }}
      <a :href="html_url" target="_blank" class="commit">{{ sha.slice(0, 7) }}</a>
      - <span class="message">{{ truncate(commit.message) }}</span><br>
      by <span class="author">
        <a :href="author.html_url" target="_blank">{{ commit.author.name }}</a>
      </span>
      at <span class="date">{{ formatDate(commit.author.date) }}</span>
    </li>
  </ul>

   <ul v-if="index_names.length > 0" id="my_index">
    <li v-for="({ index_name,crat_datetime,es_doc_count,db_row_count },index) in index_names" :key="index_name">
      {{ index }} -  {{  index_name }}
       created_at <span class="date">{{ formatDate(crat_datetime) }}</span>
      - es_doc_count <span >{{ es_doc_count}}</span><br>
      - db_row_count <span > {{ db_row_count}}</span>
    </li>
  </ul>
   </div>
  

</template>



<style scoped>
a {
  text-decoration: none;
  color: #42b883;
}
li {
  line-height: 1.5em;
  margin-bottom: 20px;
}
.author,
.date {
  font-weight: bold;
}
</style>
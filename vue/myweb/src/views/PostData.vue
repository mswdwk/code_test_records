<!--
这个示例会通过 GitHub 的 API 获取最新的 Vue Core 提交信息并将其展示为列表。
你可以在两个分支之间切换。
-->

<script>
const API_URL_2 = `http://localhost:8082/esindex/getall`

export default {
  data: () => ({
    count:0,
    index_names:[],
    formData: {
        id: '12345',
        names: []
      }
  }),

  created() {
    // 在初始化的时候进行获取
    this.submitForm()
  },

  watch: {
    // 当 count 改变时重新获取
    count: 'submitForm'
  },

  methods: {
    async submitForm() {
      const url = `${API_URL_2}`

      try {
          response = await fetch(url,{
          method: 'POST',
          body : JSON.stringify({ id : "id_1",names : ["name1","name 2 ","hello world"]}),　
          headers:{
            'Content-Type' : 'application/json',
          }
        });

        this.index_names = await response.json();

        console.log("index_names = "+this.index_names)
      }catch(error){
        console.error("error is "+error);
      }
    },
    formatDate(v) {
      return v.replace(/T|Z/g, ' ')
    }
  }
}
</script>

<template>
    <div class="PostData">
    <h1>This is an post data page</h1>
 
  <h1>Post Data Test</h1>

   <form @submit.prevent="submitForm">
     <input v-model="formData.id" type="text" placeholder="Id" name="id">
    <input v-model="formData.names" type="text" placeholder="Names" name="names">
    <button type="submit" @click="count++" id="mybution" value="my but" >
    Submit Count is: {{ count }}
    </button>
  </form>

  <p>es index names </p>
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
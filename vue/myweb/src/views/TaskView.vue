<template>
  <div class="task">
    <h1>This is an task page</h1>
     <button @click="count++">
    Count is: {{ count }}
  </button>
  <input name="id"></input>

   <p>Message is: {{ message }}</p>
    <input v-model="message" placeholder="edit me" />

   <div>Checked names: {{ checkedNames }}</div>

<input type="checkbox" id="jack" value="Jack" v-model="checkedNames" />
<label for="jack">Jack</label>

<input type="checkbox" id="john" value="John" v-model="checkedNames" />
<label for="john">John</label>

<input type="checkbox" id="mike" value="Mike" v-model="checkedNames" />
<label for="mike">Mike</label>
<tr>new line</tr>
<p></p>

  </div>
  <div>Picked: {{ picked }}

<input type="radio" id="one" value="One" v-model="picked" />
<label for="one">One</label>

<input type="radio" id="two" value="Two" v-model="picked" />
<label for="two">Two</label>
</div>

<div>Selected: {{ selected }}</div>

<select v-model="selected">
  <option disabled value="">Please select one</option>
  <option>A</option>
  <option>B</option>
  <option>C</option>
</select>
   
<li v-for="item in items">
 你好呀 {{ item.message }}
</li>
  
</template>

<script>
export default {
  data() {
    return {
      count: 0,
      checkedNames: [],
      message:"",
      picked:"One",
      selected:null,
        items: [{ message: 'Foo' }, { message: 'Bar' }]
    }
  }
}
</script>


<style scoped>
button {
  font-weight: bold;
}
</style>

<style>
@media (min-width: 1024px) {
  .task {
    min-height: 100vh;
    display: flex;
    align-items: center;
  }
}
</style>


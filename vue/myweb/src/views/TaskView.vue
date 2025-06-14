<template>
  <div class="task">
    <h1>This is an task page</h1>
     <button @click="count++">
    Count is: {{ count }}
  </button>
  </div>
</template>

<script>
export default {
  data() {
    return {
      count: 0
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
  .about {
    min-height: 100vh;
    display: flex;
    align-items: center;
  }
}
</style>


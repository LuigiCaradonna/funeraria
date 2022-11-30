<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class FinishingSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $finishings = [
            ['name' => 'Toro'],
            ['name' => 'Costa dritta']
        ];

        DB::table('finishings')->insert($finishings);
    }
}
